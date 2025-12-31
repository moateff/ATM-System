#include <stdexcept>
#include <iostream>

#include "Card.hpp"
#include "Bank.hpp"
#include "ATM.hpp"

std::uint64_t ATM::nextID = 1;

ATM::ATM(Bank& bank,
        const std::string& location, 
        double initialCash)
    : atmID(nextID++), 
      location(location), 
      availableCash(initialCash), 
      bank(bank) {
    if (initialCash < 0)
        throw std::invalid_argument("Initial ATM cash cannot be negative");
}

bool ATM::validateCardNumber(std::string cardNumber) {
    return cardNumber.size() == 3;
}

ATM::AuthenticateStatus ATM::authenticateCard(
    const std::string& cardNumber,
    const std::string& pin,
    std::shared_ptr<Card>& authenticatedCard) const
{
    for (const auto& account : bank.getAccounts()) {
        for (const auto& card : account->getCards()) {
            if (card->getCardNumber() != cardNumber)
                continue;

            if (!card->validateCard())
                return AuthenticateStatus::INVALID_CARD;

            if (!card->checkPIN(pin))
                return AuthenticateStatus::INVALID_PIN;
            if (card->isExpired())
                return AuthenticateStatus::CARD_EXPIRED;
            if (card->isBlocked())
                return AuthenticateStatus::CARD_BLOCKED;

            authenticatedCard = card;
            return AuthenticateStatus::SUCCESS;
        }
    }

    authenticatedCard = nullptr;
    return AuthenticateStatus::CARD_NOT_FOUND;
}

ATM::VerifyStatus ATM::verifyCard(const Card& card) const {
    if (!card.validateCard())
        return VerifyStatus::INVALID_CARD;
    
    if (card.isBlocked())
        return VerifyStatus::CARD_BLOCKED;

    if (card.isExpired())
        return VerifyStatus::CARD_EXPIRED;

    return VerifyStatus::SUCCESS;
}

ATM::ChangePinStatus ATM::changePIN(Card& card, const std::string& oldPin, const std::string& newPin) {
    // Step 1: Verify card status
    auto verifyStatus = verifyCard(card);
    if (verifyStatus != VerifyStatus::SUCCESS) {
        switch (verifyStatus) {
            case VerifyStatus::CARD_BLOCKED:
                return ChangePinStatus::CARD_BLOCKED;
            case VerifyStatus::CARD_EXPIRED:
                return ChangePinStatus::CARD_EXPIRED;
            default:
                return ChangePinStatus::INVALID_CARD;
        }
    }

    if (!card.checkPIN(oldPin))
        return ChangePinStatus::INVALID_OLD_PIN;

    if (!card.updatePIN(oldPin, newPin))
        return ChangePinStatus::INVALID_OLD_PIN;

    auto txn = std::make_shared<Transaction>(
        card.getAccountID(),
        Transaction::Type::PIN_CHANGE,
        0.0,
        Transaction::Status::SUCCESS
    );

    addTransaction(txn);

    auto account = bank.getAccountByID(card.getAccountID());
    if (account)
        account->addTransaction(txn);

    return ChangePinStatus::SUCCESS;
}

double ATM::displayBalance(const Card& card) {
    // Verify card first
    if (verifyCard(card) != VerifyStatus::SUCCESS)
        throw std::logic_error("Invalid or blocked card");

    // Ensure the card belongs to a valid account
    auto accountPtr = bank.getAccountByID(card.getAccountID());
    if (!accountPtr)
        throw std::logic_error("Account not found for the card");

    // Create balance inquiry transaction
    auto txn = std::make_shared<Transaction>(
        card.getAccountID(),
        Transaction::Type::BALANCE_INQUIRY,
        0.0, // no money movement
        Transaction::Status::SUCCESS
    );

    // Record transaction
    addTransaction(txn);
    accountPtr->addTransaction(txn);

    return accountPtr->getBalance();
}

// Dispense cash from the ATM and account
ATM::WithdrawStatus ATM::withdrawCash(const Card& card, double amount) {
    VerifyStatus vStatus = verifyCard(card);
    if (vStatus != VerifyStatus::SUCCESS)
        return WithdrawStatus::INVALID_CARD;

    if (amount <= 0)
        return WithdrawStatus::INVALID_AMOUNT;

    if (amount > availableCash)
        return WithdrawStatus::INSUFFICIENT_ATM_CASH;

    auto accountPtr = bank.getAccountByID(card.getAccountID());
    if (!accountPtr)
        return WithdrawStatus::INVALID_CARD;

    bool success = accountPtr->withdraw(amount);
    Transaction::Status txnStatus = success ? Transaction::Status::SUCCESS : Transaction::Status::FAILED;

    auto txn = std::make_shared<Transaction>(
        card.getAccountID(),
        Transaction::Type::WITHDRAWAL,
        amount,
        txnStatus
    );

    addTransaction(txn);
    if (success)
        accountPtr->addTransaction(txn);

    if (success)
        availableCash -= amount;

    return success ? WithdrawStatus::SUCCESS : WithdrawStatus::INSUFFICIENT_ACCOUNT_BALANCE;
}
   
// Deposit cash to the account and ATM
ATM::DepositStatus ATM::depositCash(const Card& card, double amount) {
    if (verifyCard(card) != VerifyStatus::SUCCESS)
        return DepositStatus::INVALID_CARD;

    if (amount <= 0)
        return DepositStatus::INVALID_AMOUNT;

    auto accountPtr = bank.getAccountByID(card.getAccountID());
    if (!accountPtr)
        return DepositStatus::INVALID_CARD;

    accountPtr->deposit(amount);
    availableCash += amount;
    
    auto txn = std::make_shared<Transaction>(
        card.getAccountID(),
        Transaction::Type::DEPOSIT,
        amount,
        Transaction::Status::SUCCESS
    );

    addTransaction(txn);
    accountPtr->addTransaction(txn);

    return  DepositStatus::SUCCESS;
}

ATM::TransferStatus ATM::transfer(const Card& sourceCard, std::uint64_t targetAccountID, double amount) {
    if (amount <= 0)
        return TransferStatus::INVALID_AMOUNT;

    // verify source card
    if (verifyCard(sourceCard) != VerifyStatus::SUCCESS)
        return TransferStatus::INVALID_CARD;

    auto sourceAccount = bank.getAccountByID(sourceCard.getAccountID());
    if (!sourceAccount)
        return TransferStatus::SOURCE_ACCOUNT_NOT_FOUND;

    if (sourceCard.getAccountID() == targetAccountID)
        return TransferStatus::SAME_ACCOUNT;

    auto targetAccount = bank.getAccountByID(targetAccountID);
    if (!targetAccount)
        return TransferStatus::TARGET_ACCOUNT_NOT_FOUND;

    if (sourceAccount->getBalance() < amount)
        return TransferStatus::INSUFFICIENT_BALANCE;

    // perform transfer
    sourceAccount->withdraw(amount);
    targetAccount->deposit(amount);

    // record transaction (ATM side)
    auto txn = std::make_shared<Transaction>(
        sourceAccount->getAccountID(),
        Transaction::Type::TRANSFER,
        amount,
        Transaction::Status::SUCCESS
    );

    addTransaction(txn);
    sourceAccount->addTransaction(txn);

    return TransferStatus::SUCCESS;
}

void ATM::addTransaction(const std::shared_ptr<Transaction>& transaction) {
    if (!transaction)
        throw std::invalid_argument("Transaction cannot be null");
    transactions.push_back(transaction);
}

const std::vector<std::shared_ptr<Transaction>>& ATM::getTransactions() const {
    return transactions;
}

std::uint64_t ATM::getATMID() const { return atmID; }

std::string ATM::getLocation() const { return location; }

double ATM::getAvailableCash() const { return availableCash; }

Bank& ATM::getBank() const { return bank; }

void ATM::refillCash(double amount) {
    availableCash += amount;
}

std::string ATM::toString() const {
    std::ostringstream oss;
    
    oss << "------- ATM Information -------\n";
    oss << "ATM ID        : " << atmID << "\n";
    oss << "Location      : " << location << "\n";
    oss << "Bank Name     : " << bank.getBankName() << "\n";
    oss << "Available Cash: $" << std::fixed << std::setprecision(2) << availableCash << "\n";
    oss << "Transactions  : " << transactions.size() << " recorded\n\n";

    if (!transactions.empty()) {
        size_t count = 0;
        for (auto it = transactions.rbegin(); it != transactions.rend() && count < 5; ++it, ++count) {
            oss << (*it)->toString() << "\n";
        }
        if (transactions.size() > 5)
            oss << "  ... (" << transactions.size() - 5 << " more)\n";
    }

    return oss.str();
}
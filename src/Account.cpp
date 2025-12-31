#include <sstream>
#include <stdexcept>

#include "Account.hpp"

std::uint64_t Account::nextID = 1;

Account::Account(const std::string& holderName,
                 const std::string& accNumber,
                 double initialBalance)
    : accountID(nextID++),
      accountHolderName(holderName),
      accountNumber(accNumber),
      balance(initialBalance)
{
    if (holderName.empty())
        throw std::invalid_argument("Account holder name cannot be empty");

    if (accNumber.empty())
        throw std::invalid_argument("Account number cannot be empty");

    if (initialBalance < 0)
        throw std::invalid_argument("Initial balance cannot be negative");
}

uint64_t Account::getAccountID() const {
    return accountID;
}

std::string Account::getAccountHolderName() const {
    return accountHolderName;
}

std::string Account::getAccountNumber() const {
    return accountNumber;
}

double Account::getBalance() const {
    return balance;
}

bool Account::withdraw(double amount) {
    if (amount <= 0)
        return false;

    if (amount > balance)
        return false;   // insufficient funds

    balance -= amount;
    return true;
}

void Account::deposit(double amount) {
    if (amount <= 0)
        throw std::invalid_argument("Deposit amount must be positive");

    balance += amount;
}

void Account::addCard(const std::shared_ptr<Card>& card) {
    // ensure card belongs to this account
    if (card->getAccountID() != accountID) {
        throw std::logic_error("Card does not belong to this account");
    }

    cards.push_back(card);
}

const std::vector<std::shared_ptr<Card>>& Account::getCards() const {
    return cards;
}

std::string Account::toString() const {
    std::ostringstream oss;

    oss << "---- Account Information ----\n";
    oss << "Account ID    : " << accountID << "\n";
    oss << "Holder Name   : " << accountHolderName << "\n";
    oss << "Account Number: " << accountNumber << "\n";
    oss << "Balance       : $" << balance << "\n";
    oss << "Cards         : " << cards.size() << "\n\n";

    if (!cards.empty()) {
        for (const auto& card : cards) {
            oss << card->toString();
        }
    }

    oss << "Transactions: " << transactions.size() << "\n\n";
    if (!transactions.empty()) {
        for (const auto& txn : transactions) {
            oss << txn->toString() << "\n";
        }
    }

    return oss.str();
}

void Account::addTransaction(const std::shared_ptr<Transaction>& transaction) {
    if (!transaction) {
        throw std::invalid_argument("Transaction pointer cannot be null");
    }
    transactions.push_back(transaction);
}

const std::vector<std::shared_ptr<Transaction>>& Account::getTransactions() const {
    return transactions;
}

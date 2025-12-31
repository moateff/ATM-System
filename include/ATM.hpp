#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <memory>

#include "Bank.hpp"
#include "Transaction.hpp"
#include "Card.hpp"

class Bank; // forward declaration to avoid circular include


class ATM {
public:
    enum class VerifyStatus {
        SUCCESS,
        CARD_EXPIRED,
        CARD_BLOCKED,
        INVALID_CARD
    };

    enum class WithdrawStatus {
        SUCCESS,
        INSUFFICIENT_ATM_CASH,
        INSUFFICIENT_ACCOUNT_BALANCE,
        INVALID_CARD,
        INVALID_AMOUNT
    };

    enum class DepositStatus {
        SUCCESS,
        INVALID_AMOUNT,
        INVALID_CARD
    };

    enum class TransferStatus {
        SUCCESS,
        INVALID_AMOUNT,
        INVALID_CARD,
        SAME_ACCOUNT,
        SOURCE_ACCOUNT_NOT_FOUND,
        TARGET_ACCOUNT_NOT_FOUND,
        INSUFFICIENT_BALANCE
    };

    enum class ChangePinStatus {
        SUCCESS,
        INVALID_CARD,
        INVALID_OLD_PIN,
        CARD_BLOCKED,
        CARD_EXPIRED
    };

    enum class AuthenticateStatus {
        SUCCESS,
        INVALID_CARD,
        CARD_BLOCKED,
        CARD_EXPIRED,
        INVALID_PIN,
        CARD_NOT_FOUND
    };

private:
    static std::uint64_t nextID;

    const std::uint64_t atmID;
    std::string location;
    double availableCash;

    Bank& bank;   // association (ATM does not own bank)
    std::vector<std::shared_ptr<Transaction>> transactions;

public:
    ATM(Bank& bank,
        const std::string& location, 
        double initialCash = 0.0);
    
    bool validateCardNumber(std::string cardNumber);
    AuthenticateStatus authenticateCard(const std::string& cardNumber, 
        const std::string& pin, std::shared_ptr<Card>& authenticatedCard) const;
    VerifyStatus verifyCard(const Card& card) const;
    WithdrawStatus withdrawCash(const Card& card, double amount);
    DepositStatus depositCash(const Card& card, double amount);
    TransferStatus transfer(const Card& sourceCard, std::uint64_t targetAccountID, double amount);
    ChangePinStatus changePIN(Card& card, const std::string& oldPin, const std::string& newPin);
    double displayBalance(const Card& card);

    void addTransaction(const std::shared_ptr<Transaction>& transaction);
    const std::vector<std::shared_ptr<Transaction>>& getTransactions() const;

    std::uint64_t getATMID() const;
    std::string getLocation() const;
    double getAvailableCash() const;
    Bank& getBank() const;

    void refillCash(double amount);

    std::string toString() const;
};

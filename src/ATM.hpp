#pragma once

#include <cstdint>
#include <vector>
#include "Bank.hpp"
#include "Transaction.hpp"
#include "Card.hpp"

class Bank; // forward declaration to avoid circular include


class ATM {
public:
    enum class VerifyStatus {
        OK,
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

    std::shared_ptr<Card> authenticateCard(const std::string& cardNumber, const std::string& pin);
    VerifyStatus verifyCard(const Card& card) const;
    WithdrawStatus withdrawCash(const Card& card, double amount);
    DepositStatus depositCash(const Card& card, double amount);
    TransferStatus transfer(const Card& sourceCard, std::uint64_t targetAccountID, double amount);
    ChangePinStatus changePIN(Card& card, const std::string& oldPin, const std::string& newPin);
    double displayBalance(const Card& card);

    void addTransaction(const std::shared_ptr<Transaction>& transaction);
    const std::vector<std::shared_ptr<Transaction>>& getTransactions() const;

    std::uint64_t getATMID() const { return atmID; }
    std::string getLocation() const { return location; }
    double getAvailableCash() const { return availableCash; }

    void refillCash(double amount);

    std::string toString() const;
};

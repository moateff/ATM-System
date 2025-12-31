#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <memory>

#include "Card.hpp"
#include "Transaction.hpp"

class Account {
private:
    static std::uint64_t nextID;

    const std::uint64_t accountID;
    std::string accountHolderName;
    std::string accountNumber;
    double balance;

    std::vector<std::shared_ptr<Card>> cards; // Account owns Cards
    std::vector<std::shared_ptr<Transaction>> transactions; // Account owns Transactions


public:
    Account(const std::string& holderName,
            const std::string& accountNumber,
            double initialBalance = 0.0);

    uint64_t getAccountID() const;
    std::string getAccountHolderName() const;
    std::string getAccountNumber() const;
    double getBalance() const;

    bool withdraw(double amount);
    void deposit(double amount);

    void addCard(const std::shared_ptr<Card>& card);
    const std::vector<std::shared_ptr<Card>>& getCards() const;

    void addTransaction(const std::shared_ptr<Transaction>& transaction);
    const std::vector<std::shared_ptr<Transaction>>& getTransactions() const;

    std::string toString() const;
};

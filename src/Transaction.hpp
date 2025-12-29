#pragma once

#include <cstdint>
#include <chrono>
#include <string>

class Transaction {
public:
    enum class Type {
        WITHDRAWAL,
        DEPOSIT,
        BALANCE_INQUIRY,
        TRANSFER,
        PIN_CHANGE
    };

    enum class Status {
        SUCCESS,
        FAILED
    };

private:
    static std::uint64_t nextID;

    const std::uint64_t transactionID;
    const std::uint64_t accountID;
    const std::chrono::system_clock::time_point timestamp;
    const Type type;
    const Status state;
    const double amount;

public:
    Transaction(std::uint64_t accountID,
                Type type,
                double amount,
                Status state);

    std::uint64_t getTransactionID() const;
    std::uint64_t getAccountID() const;
    Type getType() const;
    double getAmount() const;
    Status getStatus() const;
    std::chrono::system_clock::time_point getTimestamp() const;

    std::string toString() const;
};

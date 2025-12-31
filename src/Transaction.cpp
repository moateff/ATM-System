#include <sstream>
#include <ctime>
#include <stdexcept>

#include "Transaction.hpp"

std::uint64_t Transaction::nextID = 1;

Transaction::Transaction(std::uint64_t accountID,
                         Type type,
                         double amount,
                         Status status)
    : transactionID(nextID++),
      accountID(accountID),
      timestamp(std::chrono::system_clock::now()),
      type(type),
      status(status),
      amount(amount)
{
    if (amount < 0.0) {
        throw std::invalid_argument("Transaction amount cannot be negative");
    }

    if (type == Type::BALANCE_INQUIRY && amount != 0.0) {
        throw std::invalid_argument("Balance inquiry must have amount = 0");
    }
}

std::uint64_t Transaction::getTransactionID() const {
    return transactionID;
}

std::uint64_t Transaction::getAccountID() const {
    return accountID;
}

Transaction::Type Transaction::getType() const {
    return type;
}

double Transaction::getAmount() const {
    return amount;
}

Transaction::Status Transaction::getStatus() const {
    return status;
}

std::chrono::system_clock::time_point Transaction::getTimestamp() const {
    return timestamp;
}

std::string Transaction::toString() const {
    std::ostringstream oss;
    oss << "---- Transaction Information ----\n";
    oss << "Transaction ID: " << transactionID << "\n";
    oss << "Account ID    : " << accountID << "\n";
    oss << "Type          : ";
    switch (type) {
        case Type::DEPOSIT: oss << "Deposit" << "\n"; break;
        case Type::WITHDRAWAL: oss << "Withdrawal" << "\n"; break;
        case Type::BALANCE_INQUIRY: oss << "Balance Inquiry" << "\n"; break;
        case Type::TRANSFER: oss << "Transfer" << "\n"; break;
        case Type::PIN_CHANGE: oss << "Pin Change" << "\n"; break;
    }
    oss << "Amount        : " << amount << "\n";
    oss << "Status        : ";
    switch (status) {
        case Status::SUCCESS: oss << "Success" << "\n"; break;
        case Status::FAILED: oss << "Failed" << "\n"; break;
    }

    std::time_t t_time = std::chrono::system_clock::to_time_t(timestamp);
    oss << "Timestamp     : " << std::ctime(&t_time); // ctime adds newline

    return oss.str();
}

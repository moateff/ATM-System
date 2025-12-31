#pragma once

#include <string>
#include <cstdint>
#include <stdexcept>
#include <sstream>
#include <iomanip>

class Card {
public:
    struct year_month {
        int year;
        int month;

        year_month(int y, int m) {
            if (m < 1 || m > 12)
                throw std::invalid_argument("Invalid month");
            year = y;
            month = m;
        }

        bool operator<(const year_month& other) const {
            return (year < other.year) || (year == other.year && month < other.month);
        }

        std::string toString() const {
            std::ostringstream oss;
            int shortYear = year % 100;
            oss << std::setw(2) << std::setfill('0') << month 
            << "\\" 
            << std::setw(2) << std::setfill('0') << shortYear;
            return oss.str();
        }
    };

    enum class Status {
        ACTIVE,
        BLOCKED,
        EXPIRED
    };

private:
    std::string cardNumber;   
    year_month expirationDate;
    std::string cardHolderName;
    std::string bankName;
    std::string pinHash;
    const std::uint64_t accountID;
    Status status;

    static std::string hashPIN(const std::string& pin);
    std::string getMaskedCardNumber() const;

public:
    Card(const std::string& cardNumber,
         const year_month& expirationDate,
         const std::string& cardHolderName,
         const std::string& bankName,
         const std::string& pin,
         std::uint64_t accountID);

    const std::string& getCardNumber() const;
    const year_month& getExpirationDate() const;
    const std::string& getCardHolderName() const;
    const std::string& getBankName() const;
    uint64_t getAccountID() const;
    Status getStatus() const;

    bool isExpired() const;
    bool isActive() const;
    bool isBlocked() const;
    
    void blockCard();
    void unblockCard();

    bool validateCard() const;
    bool checkPIN(const std::string& pin) const;
    bool updatePIN(const std::string& oldPin, const std::string& newPin);
    std::string toString() const;
};
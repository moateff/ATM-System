#include <chrono>
#include <functional>
#include <sstream>
#include <iomanip>

#include "Card.hpp"

std::string Card::hashPIN(const std::string& pin) {
    std::hash<std::string> hasher;
    return std::to_string(hasher(pin));
}

Card::Card(const std::string& cardNumber,
           const year_month& expirationDate,
           const std::string& cardHolderName,
           const std::string& bankName,
           const std::string& pin,
           std::uint64_t accountID)
    : cardNumber(cardNumber),
      expirationDate(expirationDate),
      cardHolderName(cardHolderName),
      bankName(bankName),
      pinHash(hashPIN(pin)),
      accountID(accountID) {
    
    status = isExpired() ? Status::EXPIRED : Status::ACTIVE;
}

uint64_t Card::getAccountID() const {
    return accountID;
}

const Card::year_month& Card::getExpirationDate() const {
    return expirationDate;
}

const std::string& Card::getCardHolderName() const {
    return cardHolderName;
}

const std::string& Card::getBankName() const {
    return bankName;
}

const std::string& Card::getCardNumber() const {
    return cardNumber;
}

bool Card::validateCard() const {
    return cardNumber.size() == 3 && cardHolderName.size() > 0 && bankName.size() > 0; // && pinHash.size() == 4;
}

bool Card::isExpired() const {
    // Get current date
    auto now = std::chrono::system_clock::now();
    std::time_t tt = std::chrono::system_clock::to_time_t(now);
    std::tm local{};
#ifdef _WIN32
    localtime_s(&local, &tt);
#else
    localtime_r(&tt, &local);
#endif

    int currentYear  = local.tm_year + 1900;
    int currentMonth = local.tm_mon + 1;

    year_month current{currentYear, currentMonth};

    return expirationDate < current;
}

bool Card::isActive() const {
    return status == Status::ACTIVE && !isExpired();
}   

bool Card::isBlocked() const {
    return status == Status::BLOCKED;
}

bool Card::checkPIN(const std::string& inputPin) const {
    return pinHash == hashPIN(inputPin);
}

bool Card::updatePIN(const std::string& oldPin, const std::string& newPin) {
    if (!checkPIN(oldPin)) return false;

    pinHash = hashPIN(newPin);
    return true;
}

std::string Card::getMaskedCardNumber() const {
    std::string masked = "************";
    masked += std::string(cardNumber.data() + 12, cardNumber.size() - 12);
    return masked;
}

Card::Status Card::getStatus() const {
    return status;
}

void Card::blockCard() {
    status = Status::BLOCKED;
}

void Card::unblockCard() {
    status = isExpired() ? Status::EXPIRED : Status::ACTIVE;
}

std::string Card::toString() const {
    std::ostringstream oss;

    oss << "----- Card Information -----\n";
    oss << "Card Holder : " << cardHolderName << '\n';
    oss << "Bank        : " << bankName << '\n';
    oss << "Card Number : " << getMaskedCardNumber() << '\n';
    oss << "Expiration  : " << expirationDate.toString() << '\n';
    oss << "Account ID  : " << accountID << '\n';
    oss << "Status      : " << (isExpired() ? "Expired" : isBlocked() ? "Blocked" : "Active") << '\n';

    return oss.str();
}

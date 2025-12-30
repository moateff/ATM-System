#include <stdexcept>

#include "Bank.hpp"

Bank::Bank(const std::string& name) : bankName(name) {
    if (name.empty())
        throw std::invalid_argument("Bank name cannot be empty");
}

void Bank::addAccount(const std::shared_ptr<Account>& account) {
    if (!account)
        throw std::invalid_argument("Account cannot be null");
    accounts.push_back(account);
}

std::shared_ptr<Account> Bank::getAccountByID(std::uint64_t accountID) const {
    for (const auto& acc : accounts) {
        if (acc && acc->getAccountID() == accountID)
            return acc;
    }
    return nullptr; // not found
}

const std::vector<std::shared_ptr<Account>>& Bank::getAccounts() const {
    return accounts;
}

void Bank::addATM(const std::shared_ptr<ATM>& atm) {
    if (!atm)
        throw std::invalid_argument("ATM cannot be null");
    atms.push_back(atm);
}

const std::vector<std::shared_ptr<ATM>>& Bank::getATMs() const {
    return atms;
}

const std::string& Bank::getBankName() const {
    return bankName;
}

std::string Bank::toString() const {
    std::ostringstream oss;
    oss << "---- Bank Information ----\n";
    oss << "Name    : " << bankName << "\n";
    oss << "Accounts: " << accounts.size() << "\n";
    oss << "ATMs    : " << atms.size() << "\n";
    /*
    if (!atms.empty()) {
        for (const auto& atm : atms) {
            oss << atm->toString();
        }
    }
    */
    oss << "Accounts: " << accounts.size() << "\n\n";
    if (!accounts.empty()) {
        for (const auto& acc : accounts) {
            oss << acc->toString();
        }
    }

    return oss.str();
}
#pragma once

#include <vector>
#include <cstdint>
#include <string>
#include <memory>

#include "Account.hpp"

class ATM; // forward declaration

class Bank {
private:
    std::string bankName;

    std::vector<std::shared_ptr<Account>> accounts;
    std::vector<std::shared_ptr<ATM>> atms;

public:
    Bank(const std::string& name);

    void addAccount(const std::shared_ptr<Account>& account);
    std::shared_ptr<Account> getAccountByID(std::uint64_t accountID) const;
    const std::vector<std::shared_ptr<Account>>& getAccounts() const;

    void addATM(const std::shared_ptr<ATM>& atm);
    const std::vector<std::shared_ptr<ATM>>& getATMs() const;

    const std::string& getBankName() const;
    std::string toString() const;
};

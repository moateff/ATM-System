#include <iostream>
#include <memory>
#include "../src/Bank.hpp"
#include "../src/ATM.hpp"
#include "../src/Account.hpp"
#include "../src/Card.hpp"
#include "../src/Transaction.hpp"

int main() {
    try {
        // 1. Create Bank
        auto bank = std::make_shared<Bank>("Global Bank");

        // 2. Create Accounts
        auto account1 = std::make_shared<Account>("John Doe", "1234567890", 500.0);
        auto account2 = std::make_shared<Account>("Jane Smith", "9876543210", 1000.0);
        bank->addAccount(account1);
        bank->addAccount(account2);

        // 3. Create Cards for accounts
        Card::year_month exp1(2026, 9);  // Sept 2026
        Card::year_month exp2(2027, 12); // Dec 2027
        auto card1 = std::make_shared<Card>("1111222233334444", exp1, "John Doe", "Global Bank", "1234", account1->getAccountID());
        auto card2 = std::make_shared<Card>("5555666677778888", exp2, "Jane Smith", "Global Bank", "5678", account2->getAccountID());
        account1->addCard(card1);
        account2->addCard(card2);

        // 4. Create ATM
        auto atm = std::make_shared<ATM>(*bank, "Downtown", 2000.0);
        bank->addATM(atm);

        // --- Display initial balances ---
        std::cout << "Initial Balances:\n";
        std::cout << "John's Balance: $" << atm->displayBalance(*card1) << "\n";
        std::cout << "Jane's Balance: $" << atm->displayBalance(*card2) << "\n\n";

        // --- Deposit ---
        auto depositStatus = atm->depositCash(*card1, 300.0);
        std::cout << "Deposit: ";
        switch (depositStatus) {
            case ATM::DepositStatus::SUCCESS: std::cout << "Success"; break;
            case ATM::DepositStatus::INVALID_AMOUNT: std::cout << "Invalid Amount"; break;
            case ATM::DepositStatus::INVALID_CARD: std::cout << "Invalid Card"; break;
        }
        std::cout << "\n";

        // --- Withdraw ---
        auto withdrawStatus = atm->withdrawCash(*card2, 200.0);
        std::cout << "Withdrawal: ";
        switch (withdrawStatus) {
            case ATM::WithdrawStatus::SUCCESS: std::cout << "Success"; break;
            case ATM::WithdrawStatus::INSUFFICIENT_ATM_CASH: std::cout << "ATM Out of Cash"; break;
            case ATM::WithdrawStatus::INSUFFICIENT_ACCOUNT_BALANCE: std::cout << "Insufficient Balance"; break;
            case ATM::WithdrawStatus::INVALID_CARD: std::cout << "Invalid Card"; break;
            case ATM::WithdrawStatus::INVALID_AMOUNT: std::cout << "Invalid Amount"; break;
        }
        std::cout << "\n";

        // --- Transfer ---
        auto transferStatus = atm->transfer(*card1, account2->getAccountID(), 150.0);
        std::cout << "Transfer: ";
        switch (transferStatus) {
            case ATM::TransferStatus::SUCCESS: std::cout << "Success"; break;
            case ATM::TransferStatus::INVALID_AMOUNT: std::cout << "Invalid Amount"; break;
            case ATM::TransferStatus::INVALID_CARD: std::cout << "Invalid Card"; break;
            case ATM::TransferStatus::SAME_ACCOUNT: std::cout << "Cannot Transfer to Same Account"; break;
            case ATM::TransferStatus::SOURCE_ACCOUNT_NOT_FOUND: std::cout << "Source Account Not Found"; break;
            case ATM::TransferStatus::TARGET_ACCOUNT_NOT_FOUND: std::cout << "Target Account Not Found"; break;
            case ATM::TransferStatus::INSUFFICIENT_BALANCE: std::cout << "Insufficient Balance"; break;
        }
        std::cout << "\n";

        // --- Change PIN ---
        auto changePinStatus = atm->changePIN(*card1, "1234", "4321");
        std::cout << "Change PIN: ";
        switch (changePinStatus) {
            case ATM::ChangePinStatus::SUCCESS: std::cout << "Success"; break;
            case ATM::ChangePinStatus::INVALID_CARD: std::cout << "Invalid Card"; break;
            case ATM::ChangePinStatus::INVALID_OLD_PIN: std::cout << "Incorrect Old PIN"; break;
            case ATM::ChangePinStatus::CARD_BLOCKED: std::cout << "Card Blocked"; break;
            case ATM::ChangePinStatus::CARD_EXPIRED: std::cout << "Card Expired"; break;
        }
        std::cout << "\n";

        // --- Verify Card ---
        auto verifyStatus = atm->verifyCard(*card1);
        std::cout << "Card Verification: ";
        switch (verifyStatus) {
            case ATM::VerifyStatus::OK: std::cout << "OK"; break;
            case ATM::VerifyStatus::CARD_EXPIRED: std::cout << "Card Expired"; break;
            case ATM::VerifyStatus::CARD_BLOCKED: std::cout << "Card Blocked"; break;
            case ATM::VerifyStatus::INVALID_CARD: std::cout << "Invalid Card"; break;
        }
        std::cout << "\n";

        // --- Display balances after operations ---
        std::cout << "\nBalances After Operations:\n";
        std::cout << "John's Balance via ATM: $" << atm->displayBalance(*card1) << "\n";
        std::cout << "Jane's Balance via ATM: $" << atm->displayBalance(*card2) << "\n\n";

        // 9. Display all transactions for John
        std::cout << "Transactions for John:\n";
        for (const auto& txn : account1->getTransactions()) {
            std::cout << txn->toString() << "\n";
        }

        // 10. Display ATM details
        std::cout << "\nATM Details:\n" << atm->toString() << "\n";

    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}

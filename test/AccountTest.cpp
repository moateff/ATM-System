#include <iostream>
#include <memory>
#include "..\src\Account.hpp"
#include "..\src\Transaction.hpp"

int main() {
    try {
        // --- Create two accounts ---
        Account acc1("Alice Smith", "ACC1001", 500.0);
        Account acc2("Bob Johnson", "ACC1002", 300.0);

        std::cout << "Initial state:\n";
        std::cout << acc1.toString() << "\n";
        std::cout << acc2.toString() << "\n";

        // --- Deposit $200 to Alice's account ---
        double depositAmount = 200.0;
        bool success = acc1.deposit(depositAmount);

        auto depositTxn = std::make_shared<Transaction>(
            acc1.getAccountID(), Transaction::Type::DEPOSIT, depositAmount, 
            success ? Transaction::Status::SUCCESS : Transaction::Status::FAILED
        );
        acc1.addTransaction(depositTxn);

        std::cout << "\nAfter depositing $200 to Alice:\n";
        std::cout << acc1.toString() << "\n";

        // --- Withdraw $150 from Alice's account ---
        double withdrawAmount = 150.0;
        success = acc1.withdraw(withdrawAmount);

        auto withdrawTxn = std::make_shared<Transaction>(
            acc1.getAccountID(), Transaction::Type::WITHDRAWAL, withdrawAmount,
            success ? Transaction::Status::SUCCESS : Transaction::Status::FAILED
        );
        acc1.addTransaction(withdrawTxn);

        std::cout << "\nAfter withdrawing $150 from Alice:\n";
        std::cout << acc1.toString() << "\n";

        // --- Transfer $100 from Alice to Bob ---
        double transferAmount = 100.0;
        success = acc1.withdraw(transferAmount);
        auto transferOutTxn = std::make_shared<Transaction>(
            acc1.getAccountID(), Transaction::Type::WITHDRAWAL, transferAmount,
            success ? Transaction::Status::SUCCESS : Transaction::Status::FAILED
        );
        acc1.addTransaction(transferOutTxn);

        if (success) {
            success = acc2.deposit(transferAmount);

            auto transferInTxn = std::make_shared<Transaction>(
                acc2.getAccountID(), Transaction::Type::DEPOSIT, transferAmount, 
                success ? Transaction::Status::SUCCESS : Transaction::Status::FAILED
            );
            acc2.addTransaction(transferInTxn);
        }

        std::cout << "\nAfter transferring $100 from Alice to Bob:\n";
        std::cout << acc1.toString() << "\n";
        std::cout << acc2.toString() << "\n";

    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
    }

    return 0;
}

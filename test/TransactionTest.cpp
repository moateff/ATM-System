#include <iostream>
#include "..\src\Transaction.hpp"

int main() {
    try {
        Transaction t1(5001, Transaction::Type::DEPOSIT, 250.0, Transaction::Status::SUCCESS);
        Transaction t2(5001, Transaction::Type::WITHDRAWAL, 100.0, Transaction::Status::FAILED);
        Transaction t3(5002, Transaction::Type::BALANCE_INQUIRY, 0.0, Transaction::Status::SUCCESS);

        std::cout << t1.toString() << std::endl;
        std::cout << t2.toString() << std::endl;
        std::cout << t3.toString() << std::endl;

    } catch (const std::exception& ex) {
        std::cerr << "Error creating transaction: " << ex.what() << std::endl;
    }

    return 0;
}
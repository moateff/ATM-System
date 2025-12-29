#include <iostream>
#include "..\src\Card.hpp"

int main() {
    Card card("1234567890123456", Card::year_month(2025, 9), "Atef", "Al-ahly Bank", "1234", 9876543210);

    std::cout << card.toString() << std::endl;

    if (card.validateCard()) {
        std::cout << "Card is valid." << std::endl;
    } else {
        std::cout << "Card is invalid." << std::endl;
    }

    if (card.checkPIN("2345")) {
        std::cout << "PIN is correct." << std::endl;
    } else {
        std::cout << "PIN is incorrect." << std::endl;
    }
    
    return 0;
}

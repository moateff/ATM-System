#include <memory>

#include "Bank.hpp"
#include "Account.hpp"
#include "Card.hpp"
#include "ATM.hpp"
#include "UI.hpp"

int main() {
    auto bank = std::make_shared<Bank>("Global Bank");

    auto acc1 = std::make_shared<Account>("John Doe", "ACC001", 1000.0);
    auto acc2 = std::make_shared<Account>("Jane Smith", "ACC002", 500.0);

    bank->addAccount(acc1);
    bank->addAccount(acc2);

    auto card1 = std::make_shared<Card>(
        "123", Card::year_month(2027, 12), "John Doe",
        "Global Bank", "1111", acc1->getAccountID()
    );

    auto card2 = std::make_shared<Card>(
        "456", Card::year_month(2026, 6), "Jane Smith",
        "Global Bank", "2222", acc2->getAccountID()
    );

    auto card3 = std::make_shared<Card>(
        "789", Card::year_month(2025, 6), "Jane Smith",
        "Global Bank", "3333", acc2->getAccountID()
    );

    acc1->addCard(card1);
    acc2->addCard(card2);
    acc2->addCard(card3);

    auto atm = std::make_shared<ATM>(*bank, "Main Branch", 5000.0);
    bank->addATM(atm);

    UI ui(atm);
    ui.run();

    return 0;
}

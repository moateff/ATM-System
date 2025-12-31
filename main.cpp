#include <memory>

#include "Bank.hpp"
#include "Account.hpp"
#include "Card.hpp"
#include "ATM.hpp"
#include "UI.hpp"

int main() {
    auto bank = std::make_shared<Bank>("Al-Ahly Bank");

    auto acc1 = std::make_shared<Account>("Mohamed Atef", "ACC001", 1000.0);
    auto acc2 = std::make_shared<Account>("Hossam Abdullah", "ACC002", 500.0);

    bank->addAccount(acc1);
    bank->addAccount(acc2);

    auto validCard1 = std::make_shared<Card>(
        "123", Card::year_month(2027, 12), "Mohamed Atef",
        "Al-Ahly Bank", "1111", acc1->getAccountID()
    );

    // invalid card (no name)
    auto invalidCard1 = std::make_shared<Card>(
        "147", Card::year_month(2027, 12), "", 
        "", "2222", acc1->getAccountID() 
    );

    auto validCard2 = std::make_shared<Card>(
        "456", Card::year_month(2026, 6), "Hossam Abdullah",
        "Al-Ahly Bank", "3333", acc2->getAccountID()
    );

    // expired card (2025)
    auto expiredCard3 = std::make_shared<Card>(
        "258", Card::year_month(2025, 6), "Hossam Abdullah",
        "Al-Ahly Bank", "4444", acc2->getAccountID()
    );

    acc1->addCard(validCard1);
    acc1->addCard(invalidCard1);
    acc2->addCard(validCard2);
    acc2->addCard(expiredCard3);

    auto atm = std::make_shared<ATM>(*bank, "Main Branch", 5000.0);
    bank->addATM(atm);

    UI ui(atm);
    ui.run();

    return 0;
}

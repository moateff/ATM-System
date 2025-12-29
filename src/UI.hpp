#pragma once

#include <memory>
#include "../src/ATM.hpp"
#include "../src/Card.hpp"

class UI {
public:
    enum class ATMPage {
        InsertCard,
        MainMenu,
        Withdraw,
        Deposit,
        Transfer,
        Balance,
        ChangePIN,
        Exit
    };

    struct CardCredentials {
        std::string cardNumber;
        std::string pin;
    };

private:
    std::shared_ptr<ATM> atm;
    std::shared_ptr<Card> currentCard;
    ATMPage currentPage;

    void insertCardPage();
    void mainMenuPage();
    void withdrawPage();
    void depositPage();
    void transferPage();
    void balancePage();
    void changePINPage();

    CardCredentials drawInsertCardPage();
    int drawMainMenuPage();

public:
    explicit UI(std::shared_ptr<ATM> atm);
    void run();
};

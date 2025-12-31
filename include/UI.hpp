#pragma once
#include <memory>
#include <vector>
#include <string>

#include "ATM.hpp"
#include "Card.hpp"
#include "Keyboard.hpp"

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

class UI {
public:
    UI(std::shared_ptr<ATM> atm);

    void run();

    enum class Color {
        BLACK  = 0,
        RED    = 4,
        GREEN  = 2,
        YELLOW = 6,
        WHITE  = 7
    };

private:
    // Screen
    int x = 10, y = 3;
    int width = 40, height = 12;

    std::shared_ptr<ATM> atm;
    std::shared_ptr<Card> currentCard;
    ATMPage currentPage;
    Keyboard keyboard;
    int invalidPINcounter = 0;

    // Pages
    void insertCardPage();
    void mainMenuPage();
    void withdrawPage();
    void depositPage();
    void transferPage();
    void changePinPage();
    void balancePage();

    // Utility
    void showMessage(const std::string& message, 
                    Color color = Color::GREEN, 
                    int durationSeconds = 1);

    // Forms
    std::vector<std::string> insertCardForm();
    std::vector<std::string> withdrawForm();
    std::vector<std::string> depositForm();
    std::vector<std::string> transferForm();
    std::vector<std::string> changePinForm();
};

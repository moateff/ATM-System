#include "UI.hpp"
#include <iostream>

#include "../utils/AnsiRenderer.hpp"
#include "../utils/TextField.hpp"
#include "../utils/Keyboard.hpp"
#include "ATMMenu.hpp"

UI::UI(std::shared_ptr<ATM> atm)
    : atm(std::move(atm)), currentPage(ATMPage::InsertCard) {}

UI::CardCredentials UI::drawInsertCardPage() {
    LinuxKeyboard keyboard;

    AnsiRenderer::clear();

    // Outer form
    AnsiRenderer::invertOn();
    AnsiRenderer::drawBox(10, 3, 40, 12);

    AnsiRenderer::drawText(24, 4, "INSERT CARD");
    AnsiRenderer::drawText(14, 7, "Card Number:");
    AnsiRenderer::drawText(14, 9, "PIN:");
    AnsiRenderer::drawText(18, 12, "Press ENTER to continue");

    AnsiRenderer::invertOff();

    AnsiRenderer::drawBox(28, 7, 17, 1);
    AnsiRenderer::drawBox(28, 9, 17, 1);

    // Input fields
    TextField cardField(
        28, 7, 16, keyboard,
        [](char c) { return std::isdigit(c); }
    );

    TextField pinField(
        28, 9, 4, keyboard,
        [](char c) { return std::isdigit(c); },
        true   // masked
    );

    CardCredentials creds;
    creds.cardNumber = cardField.read();
    creds.pin = pinField.read();

    return creds;
}

void UI::insertCardPage() {
    auto creds = drawInsertCardPage();

    auto card = atm->authenticateCard(creds.cardNumber, creds.pin);
    if (!card)  {
        std::cout << "Invalid card or PIN.\n";
        return;
    }

    currentCard = card;
    std::cout << "Card accepted.\n";
    currentPage = ATMPage::MainMenu;
}

int UI::drawMainMenuPage() {
    LinuxKeyboard keyboard;

    AnsiRenderer::clear();

    ATMMenu menu(10, 3, 40, 12, keyboard);
    int choice = menu.showMenu(); // returns 0..5

    return choice;
}

void UI::mainMenuPage() {
    int choice = drawMainMenuPage();

    // Map choice index to ATMPage
    const std::vector<ATMPage> pageMap = {
        ATMPage::Withdraw,
        ATMPage::Deposit,
        ATMPage::Transfer,
        ATMPage::Balance,
        ATMPage::ChangePIN,
        ATMPage::Exit
    };

    if (choice >= 0 && choice < (int)pageMap.size()) {
        currentPage = pageMap[choice];
    } else {
        currentPage = ATMPage::Exit;
    }
}


void UI::withdrawPage() {
    double amount;
    std::cout << "Withdraw amount: ";
    std::cin >> amount;

    auto status = atm->withdrawCash(*currentCard, amount);
    switch (status) {
        case ATM::WithdrawStatus::SUCCESS:
            std::cout << "Withdrawal successful.\n"; break;
        case ATM::WithdrawStatus::INSUFFICIENT_ACCOUNT_BALANCE:
            std::cout << "Insufficient account balance.\n"; break;
        case ATM::WithdrawStatus::INSUFFICIENT_ATM_CASH:
            std::cout << "ATM out of cash.\n"; break;
        case ATM::WithdrawStatus::INVALID_AMOUNT:
            std::cout << "Invalid amount.\n"; break;
        case ATM::WithdrawStatus::INVALID_CARD:
            std::cout << "Invalid card.\n"; break;
    }

    currentPage = ATMPage::MainMenu;
}

void UI::depositPage() {
    double amount;
    std::cout << "Deposit amount: ";
    std::cin >> amount;

    auto status = atm->depositCash(*currentCard, amount);
    switch (status) {
        case ATM::DepositStatus::SUCCESS:
            std::cout << "Deposit successful.\n"; break;
        case ATM::DepositStatus::INVALID_AMOUNT:
            std::cout << "Invalid amount.\n"; break;
        case ATM::DepositStatus::INVALID_CARD:
            std::cout << "Invalid card.\n"; break;
    }

    currentPage = ATMPage::MainMenu;
}

void UI::transferPage() {
    std::uint64_t targetID;
    double amount;

    std::cout << "Target account ID: ";
    std::cin >> targetID;
    std::cout << "Amount: ";
    std::cin >> amount;

    auto status = atm->transfer(*currentCard, targetID, amount);
    switch (status) {
        case ATM::TransferStatus::SUCCESS:
            std::cout << "Transfer successful.\n"; break;
        case ATM::TransferStatus::INSUFFICIENT_BALANCE:
            std::cout << "Insufficient balance.\n"; break;
        case ATM::TransferStatus::SAME_ACCOUNT:
            std::cout << "Cannot transfer to same account.\n"; break;
        case ATM::TransferStatus::TARGET_ACCOUNT_NOT_FOUND:
            std::cout << "Target account not found.\n"; break;
        case ATM::TransferStatus::INVALID_AMOUNT:
            std::cout << "Invalid amount.\n"; break;
        default:
            std::cout << "Transfer failed.\n";
    }

    currentPage = ATMPage::MainMenu;
}

void UI::balancePage() {
    std::cout << "Current Balance: $"
              << atm->displayBalance(*currentCard) << "\n";
    currentPage = ATMPage::MainMenu;
}

void UI::changePINPage() {
    std::string oldPin, newPin;

    std::cout << "Old PIN: ";
    std::cin >> oldPin;
    std::cout << "New PIN: ";
    std::cin >> newPin;

    auto status = atm->changePIN(*currentCard, oldPin, newPin);
    switch (status) {
        case ATM::ChangePinStatus::SUCCESS:
            std::cout << "PIN changed successfully.\n"; break;
        case ATM::ChangePinStatus::INVALID_OLD_PIN:
            std::cout << "Invalid old PIN.\n"; break;
        case ATM::ChangePinStatus::CARD_BLOCKED:
            std::cout << "Card blocked.\n"; break;
        case ATM::ChangePinStatus::CARD_EXPIRED:
            std::cout << "Card expired.\n"; break;
        default:
            std::cout << "Failed to change PIN.\n";
    }

    currentPage = ATMPage::MainMenu;
}

void UI::run() {
    while (true) {
        switch (currentPage) {
            case ATMPage::InsertCard: insertCardPage(); break;
            case ATMPage::MainMenu: mainMenuPage(); break;
            case ATMPage::Withdraw: withdrawPage(); break;
            case ATMPage::Deposit: depositPage(); break;
            case ATMPage::Transfer: transferPage(); break;
            case ATMPage::Balance: balancePage(); break;
            case ATMPage::ChangePIN: changePINPage(); break;
            case ATMPage::Exit: currentPage = ATMPage::InsertCard; break;
            default: break;
        }
    }

    std::cout << "\nThank you for using the ATM.\n";
}

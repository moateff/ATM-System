#include <sstream>
#include <iomanip>
#include <unistd.h>
#include <iostream>
#include <iostream>
#include <vector>
#include <string>
#include <cctype>

#include "UI.hpp"
#include "AnsiRenderer.hpp"
#include "Form.hpp"
#include "Menu.hpp"
#include "Keyboard.hpp"
#include "TextField.hpp"
#include "TextBuffer.hpp"
#include "Label.hpp"

UI::UI(std::shared_ptr<ATM> atm)
    : atm(atm), currentCard(nullptr), currentPage(ATMPage::InsertCard), keyboard(Keyboard()) {}

void UI::run() {
    while (true) {
        switch (currentPage) {
            case ATMPage::InsertCard: insertCardPage(); break;
            case ATMPage::MainMenu:   mainMenuPage(); break;
            case ATMPage::Withdraw:   withdrawPage(); break;
            case ATMPage::Deposit:    depositPage(); break;
            case ATMPage::Transfer:   transferPage(); break;
            case ATMPage::Balance:    balancePage(); break;
            case ATMPage::ChangePIN:  changePinPage(); break;
            case ATMPage::Exit:       currentPage = ATMPage::InsertCard; break;
        }
    }

    AnsiRenderer::clear();
    AnsiRenderer::move(0, 0);
}

// Pages 
void UI::insertCardPage() {
    auto values = insertCardForm();

    // Ensure both card number and PIN are entered
    if (values.size() != 2) { 
        currentPage = ATMPage::InsertCard;
        return; 
    }

    if (values[0].empty() || values[1].empty()) { 
        currentPage = ATMPage::InsertCard;
        return; 
    }

    // Validate card number
    if (!atm->validateCardNumber(values[0])) {
        showMessage("INVALID CARD NUMBER", Color::RED);
        currentPage = ATMPage::InsertCard;
        return;
    }

    // Authenticate card
    std::shared_ptr<Card> cardPtr;
    auto status = atm->authenticateCard(values[0], values[1], cardPtr);

    // Handle authentication status
    switch (status) {
        case ATM::AuthenticateStatus::SUCCESS:
            currentCard = cardPtr;
            showMessage("ACCESS GRANTED");
            currentPage = ATMPage::MainMenu;
            break;

        case ATM::AuthenticateStatus::CARD_EXPIRED:
            showMessage("CARD EXPIRED", Color::YELLOW);
            currentPage = ATMPage::InsertCard;
            break;

        case ATM::AuthenticateStatus::CARD_BLOCKED:
            showMessage("CARD BLOCKED", Color::RED);
            currentPage = ATMPage::InsertCard;
            break;

        case ATM::AuthenticateStatus::INVALID_PIN:
            /*
            if (invalidPINcounter >= 3) {
                showMessage("CARD BLOCKED TEMPORARILY", Color::RED);
                currentCard->blockCard();
            } else if (invalidPINcounter == 2) {
                showMessage("INVALID PIN " + std::to_string(invalidPINcounter) + " TIMES", Color::YELLOW);
            } else {
                showMessage("INVALID PIN " + std::to_string(invalidPINcounter) + " TIMES", Color::YELLOW);
            }
            */
            showMessage("INVALID PIN", Color::RED);
            currentPage = ATMPage::InsertCard;
            break;

        case ATM::AuthenticateStatus::INVALID_CARD:
            showMessage("INVALID CARD", Color::RED);
            currentPage = ATMPage::InsertCard;
            break;
        
        case ATM::AuthenticateStatus::CARD_NOT_FOUND:
            showMessage("CARD NOT FOUND", Color::RED);
            currentPage = ATMPage::InsertCard;
            break;
    }
}

void UI::mainMenuPage() {
    std::vector<std::string> options = {
        "WITHDRAW", "DEPOSIT",
        "TRANSFER", "BALANCE ENQUIRY",
        "CHANGE PIN", "EXIT"
    };
    Menu menu(x, y,  width, height, "ATM MENU", options, 3, 2);
    int choice = menu.show(keyboard);

    switch(choice) {
        case 0: currentPage = ATMPage::Withdraw; break;
        case 1: currentPage = ATMPage::Deposit; break;
        case 2: currentPage = ATMPage::Transfer; break;
        case 3: currentPage = ATMPage::Balance; break;
        case 4: currentPage = ATMPage::ChangePIN; break;
        case 5: currentPage = ATMPage::Exit; break;
    }
}

void UI::withdrawPage() {
    auto values = withdrawForm();

    if (values.size() != 1) { 
        currentPage = ATMPage::MainMenu; 
        return; 
    }

    if (values[0].empty()) { 
        showMessage("ENTER AMOUNT", Color::YELLOW);
        currentPage = ATMPage::Withdraw; 
        return; 
    }

    double amount = std::stod(values[0]);
    auto status = atm->withdrawCash(*currentCard, amount);

    switch(status) {
        case ATM::WithdrawStatus::SUCCESS: showMessage("WITHDRAWAL SUCCESS"); break;
        case ATM::WithdrawStatus::INSUFFICIENT_ACCOUNT_BALANCE: showMessage("INSUFFICIENT BALANCE", Color::RED); break;
        case ATM::WithdrawStatus::INSUFFICIENT_ATM_CASH: showMessage("ATM OUT OF CASH", Color::RED); break;
        case ATM::WithdrawStatus::INVALID_AMOUNT: showMessage("INVALID AMOUNT", Color::RED); break;
        case ATM::WithdrawStatus::INVALID_CARD: showMessage("INVALID CARD", Color::RED); break;
    }

    currentPage = ATMPage::MainMenu;
}

void UI::depositPage() {
    auto values = depositForm();
    
    if (values.size() != 1) { 
        currentPage = ATMPage::MainMenu; 
        return; 
    }

    if (values[0].empty()) { 
        showMessage("ENTER AMOUNT", Color::YELLOW);
        currentPage = ATMPage::Deposit; 
        return; 
    }

    double amount = std::stod(values[0]);
    auto status = atm->depositCash(*currentCard, amount);

    switch(status) {
        case ATM::DepositStatus::SUCCESS: showMessage("DEPOSIT SUCCESS"); break;
        case ATM::DepositStatus::INVALID_AMOUNT: showMessage("INVALID AMOUNT", Color::RED); break;
        case ATM::DepositStatus::INVALID_CARD: showMessage("INVALID CARD", Color::RED); break;
    }

    currentPage = ATMPage::MainMenu;
}

void UI::transferPage() {
    auto values = transferForm();

    if (values.size() != 2) { 
        currentPage = ATMPage::MainMenu; 
        return; 
    }

    if (values[0].empty()) { 
        showMessage("ENTER TARGET ACCOUNT", Color::YELLOW);
        currentPage = ATMPage::Transfer; 
        return; 
    }

    if (values[1].empty()) { 
        showMessage("ENTER AMOUNT", Color::YELLOW);
        currentPage = ATMPage::Transfer; 
        return; 
    }

    std::uint64_t targetAccount = std::stoull(values[0]);
    double amount = std::stod(values[1]);

    auto status = atm->transfer(*currentCard, targetAccount, amount);

    switch(status) {
        case ATM::TransferStatus::SUCCESS: showMessage("TRANSFER SUCCESS"); break;
        case ATM::TransferStatus::INVALID_AMOUNT: showMessage("INVALID AMOUNT", Color::RED); break;
        case ATM::TransferStatus::INVALID_CARD: showMessage("INVALID CARD", Color::RED); break;
        case ATM::TransferStatus::SAME_ACCOUNT: showMessage("CANNOT TRANSFER TO SAME ACCOUNT", Color::YELLOW); break;
        case ATM::TransferStatus::SOURCE_ACCOUNT_NOT_FOUND: showMessage("SOURCE ACCOUNT NOT FOUND", Color::RED); break;
        case ATM::TransferStatus::TARGET_ACCOUNT_NOT_FOUND: showMessage("TARGET ACCOUNT NOT FOUND", Color::RED); break;
        case ATM::TransferStatus::INSUFFICIENT_BALANCE: showMessage("INSUFFICIENT BALANCE", Color::RED); break;
    }

    currentPage = ATMPage::MainMenu;
}

void UI::changePinPage() {
    auto values = changePinForm();

    if (values.size() != 2) { 
        currentPage = ATMPage::MainMenu; 
        return; 
    }

    if (values[0].empty()) { 
        showMessage("ENTER OLD PIN", Color::YELLOW);
        currentPage = ATMPage::ChangePIN; 
        return; 
    }

    if (values[1].empty()) { 
        showMessage("ENTER NEW PIN", Color::YELLOW);
        currentPage = ATMPage::ChangePIN; 
        return; 
    }

    std::string oldPin = values[0];
    std::string newPin = values[1];

    auto status = atm->changePIN(*currentCard, oldPin, newPin);

    switch(status) {
        case ATM::ChangePinStatus::SUCCESS: showMessage("PIN CHANGED"); break;
        case ATM::ChangePinStatus::INVALID_CARD: showMessage("INVALID CARD", Color::RED); break;
        case ATM::ChangePinStatus::INVALID_OLD_PIN: showMessage("INVALID OLD PIN", Color::RED); break;
        case ATM::ChangePinStatus::CARD_BLOCKED: showMessage("CARD BLOCKED", Color::RED); break;
        case ATM::ChangePinStatus::CARD_EXPIRED: showMessage("CARD EXPIRED", Color::YELLOW); break;
    }

    currentPage = ATMPage::MainMenu;
}

void UI::balancePage() {
    double balance = atm->displayBalance(*currentCard);

    AnsiRenderer::clear();
    AnsiRenderer::hideCursor();

    int x = 10, y = 3, w = 40, h = 12;
    AnsiRenderer::invertOn();
    AnsiRenderer::drawBox(x, y, w, h);
    AnsiRenderer::drawText(x + (w - 15)/2, y + 2, "BALANCE ENQUIRY");

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << balance;
    AnsiRenderer::drawText(x + 4, y + 5, "CURRENT BALANCE:");
    AnsiRenderer::drawText(x + 22, y + 5, "$ " + oss.str());

    std::string footer = "Press ENTER to continue";
    AnsiRenderer::drawText(x + (w - footer.size())/2, y + h - 3, footer);

    AnsiRenderer::flush();

    // Wait for enter
    std::cin.get();

    AnsiRenderer::invertOff();
    AnsiRenderer::showCursor();

    currentPage = ATMPage::MainMenu;
}

// Utils
void UI::showMessage(const std::string& message, Color color, int durationSeconds) {
    AnsiRenderer::clear();
    AnsiRenderer::hideCursor();

    if (color == Color::RED) std::cout << "\033[41;97m"; // red background
    else if (color == Color::YELLOW) std::cout << "\033[30;103m"; // yellow background
    else std::cout << "\033[42;97m"; // green background

    AnsiRenderer::drawBox(x, y, width, height);
    AnsiRenderer::drawText(x + (width - message.size())/2, y + (height - 1)/2, message);

    std::cout << "\033[0m" << std::flush;
    sleep(durationSeconds);

    AnsiRenderer::showCursor();
}

// Forms 
std::vector<std::string> UI::insertCardForm() {
    AnsiRenderer::clear();

    Form form(
        x, y,                  
        width, height,            
        "INSERT CARD",             // header
        "Press ENTER to continue"  // footer
    );

    // Card number field
    form.addField(
        Label(
            14, 7,
            "CARD NUMBER:"
        ), TextField(
            29, 7,                 // input position
            16, 10,                // width, capacity
            [](char c) { return c >= '0' && c <= '9'; },
            false
        )
    );

    // PIN field (masked)
    form.addField(
        Label (
            14, 9, 
            "PIN:"
        ), TextField(
            29, 9,
            16, 4,
            [](char c) { return c >= '0' && c <= '9'; },
            true
        )
    );

    return form.run(keyboard);
}

std::vector<std::string> UI::changePinForm() {
    AnsiRenderer::clear();

    Form form(
        x, y,                
        width, height,       
        "CHANGE PIN",        // header
        "ENTER TO CONFIRM   ESC TO CANCEL" //  footer
    );

    // OLD PIN
    form.addField(
        Label(14, 7, "OLD PIN:"), 
        TextField(
            29, 7,
            16, 4,
            [](char c) { return c >= '0' && c <= '9'; },
            true
        )
    );

    // NEW PIN
    form.addField(
        Label(14, 9, "NEW PIN:"), 
        TextField(
            29, 9,
            16, 4,
            [](char c) { return c >= '0' && c <= '9'; },
            true
        )
    );

    return form.run(keyboard);
}

std::vector<std::string> UI::withdrawForm() {
    AnsiRenderer::clear();

    Form form(
        x, y,                      
        width, height,             
        "WITHDRAW CASH",           // header
        "Press ENTER to continue"  // footer
    );

    // Amount field
    form.addField(
        Label(14, 7, "ENTER AMOUNT:"),
        TextField(
            20, 9,                 // input position
            20, 10,                // width, capacity
            [](char c) { return c >= '0' && c <= '9'; },
            false
        )
    );

    return form.run(keyboard);
}

std::vector<std::string> UI::depositForm() {
    AnsiRenderer::clear();

    Form form(
        x, y,                      
        width, height,             
        "DEPOSIT CASH",            // header
        "Press ENTER to continue"  // footer
    );

    // Amount field
    form.addField(
        Label(14, 7, "ENTER AMOUNT:"),
        TextField(
            20, 9,                 // input position
            20, 10,                // width, capacity
            [](char c) { return c >= '0' && c <= '9'; },
            false
        )
    );

    return form.run(keyboard);
}

std::vector<std::string> UI::transferForm() {
    AnsiRenderer::clear();

    Form form(
        x, y,                      
        width, height,             
        "TRANSFER FUNDS",           // header
        "Press ENTER to continue"   // footer
    );

    // Target account number
    form.addField(
        Label(14, 7, "TARGET ACCOUNT:"),
        TextField(
            29, 7,                 // input position
            16, 4,                // width, capacity
            [](char c){ return std::isdigit(c); }, 
            false
        )
    );

    // Amount
    form.addField(
        Label(14, 9, "AMOUNT:"),
        TextField(
            29, 9,
            16, 10,
            [](char c){ return std::isdigit(c) || c == '.'; }, // allow decimal point
            false
        )
    );

    return form.run(keyboard);
}

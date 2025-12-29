#include "ATMMenu.hpp"
#include "../utils/AnsiRenderer.hpp"
#include "../utils/Keyboard.hpp"
#include <iostream>

ATMMenu::ATMMenu(int x, int y, int width, int height, Keyboard& kb)
    : x(x), y(y), width(width), height(height), keyboard(kb), currentChoice(0) {}

void ATMMenu::drawMenu() {
    AnsiRenderer::move(x, y);

    // Draw outer box
    AnsiRenderer::invertOn();
    AnsiRenderer::drawBox(x, y, width, height);

    // Draw title
    AnsiRenderer::drawText(x + width/2 - 5, y + 1, "ATM MENU");
    AnsiRenderer::invertOff();

    // Draw items in 3x2 grid
    for (size_t i = 0; i < items.size(); ++i) {
        bool selected = (i == currentChoice);

        int row = i / 2;
        int col = i % 2;

        int itemX = x + col * (width/2);
        int itemY = y + 4 + row * 3;

        if (!selected)
            AnsiRenderer::invertOn();

        AnsiRenderer::drawBox(itemX, itemY - 1, width / 2, 3);
        AnsiRenderer::drawText(itemX, itemY, items[i]);

        if (!selected)
            AnsiRenderer::invertOff();
    }
}

int ATMMenu::showMenu() {
    AnsiRenderer::hideCursor();

    AnsiRenderer::clear();
    drawMenu();

    while (true) {        
        int key = keyboard.readKey();

        /*
        AnsiRenderer::drawText(0, y + height + 1, "You pressed:");

        switch (key) {
            case KEY_LEFT:        std::cout << "LEFT ARROW"; break;
            case KEY_RIGHT:       std::cout << "RIGHT ARROW"; break;
            case KEY_UP:          std::cout << "UP ARROW"; break;
            case KEY_DOWN:        std::cout << "DOWN ARROW"; break;
            case KEY_ENTER:       std::cout << "ENTER"; break;
            case KEY_BACKSPACE:   std::cout << "BACKSPACE"; break;
            case KEY_DELETE:      std::cout << "DELETE"; break;
            default:              std::cout << static_cast<char>(key); break;
        }

        std::cout << std::endl;
        */
        switch(key) {
            case KEY_LEFT:
                if (currentChoice % 2 == 1) currentChoice--;
                break;
            case KEY_RIGHT:
                if (currentChoice % 2 == 0 && currentChoice + 1 < (int)items.size()) 
                currentChoice++;
                break;
            case KEY_UP:
                if (currentChoice >= 2) currentChoice -= 2;
                break;
            case KEY_DOWN:
                if (currentChoice + 2 < (int)items.size()) 
                currentChoice += 2;
                break;
            case KEY_ENTER:
                return currentChoice;
        }

        drawMenu();
    }

    return currentChoice;
}

std::string ATMMenu::getItemName(int index) {
    if (index >= 0 && index < (int)items.size()) {
        return items[index];
    }
    return "";
}
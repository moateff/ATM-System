#pragma once

#include <vector>
#include <string>

// Forward declaration to avoid circular includes
class Keyboard;

class ATMMenu {
public:
    ATMMenu(int x, int y, int width, int height, Keyboard& kb);

    // Shows menu, handles navigation, returns selected index
    int showMenu();

    // Returns item name by index
    std::string getItemName(int index);

private:
    int x;
    int y;
    int width;
    int height;

    Keyboard& keyboard;

    std::vector<std::string> items = {
        "      WITHDRAW     ", "      DEPOSIT      ",
        "      TRANSFER     ", "  BALANCE ENQUIRY  ",
        "        EXIT       ", "     CHANGE PIN    "
    };

    int currentChoice;

    // Draws the menu UI
    void drawMenu();
};

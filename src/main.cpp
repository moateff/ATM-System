#include <iostream>
#include "ATMMenu.hpp"
#include "../utils/Keyboard.hpp"
#include "../utils/AnsiRenderer.hpp"

int main() {
    // Create concrete keyboard for Linux
    LinuxKeyboard keyboard;

    // Menu position and size
    int x = 10;
    int y = 3;
    int width = 40;
    int height = 13;

    // Hide cursor for menu display
    AnsiRenderer::hideCursor();
    AnsiRenderer::clear();

    // Create and show menu
    ATMMenu menu(x, y, width, height, keyboard);
    int choice = menu.showMenu();

    // Show cursor again
    AnsiRenderer::showCursor();
    AnsiRenderer::clear();

    // Output the choice
    std::cout << "You selected menu index: " << choice
              << " (" << menu.getItemName(choice) << ")\n";

    return 0;
}

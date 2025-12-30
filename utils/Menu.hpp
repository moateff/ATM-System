#pragma once
#include <vector>
#include <string>
#include "AnsiRenderer.hpp"
#include "Keyboard.hpp"

class Menu {
public:
    Menu(int x, int y, int width, int height,
         const std::string& header,
         const std::vector<std::string>& options,
         int rows, int cols);

    int show(Keyboard& keyboard);  // returns index of selected option

private:
    int x, y, width, height;
    std::string header;
    std::vector<std::string> options;
    int rows, cols;
    int currentChoice;

    void drawMenu(int prevChoice = -1);
};

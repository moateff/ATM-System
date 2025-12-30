#include "Menu.hpp"
#include <iostream>

Menu::Menu(int x, int y, int width, int height,
           const std::string& header,
           const std::vector<std::string>& options,
           int rows, int cols)
    : x(x), y(y), width(width), height(height),
      header(header), options(options), rows(rows), cols(cols),
      currentChoice(0)
{
    if ((int)options.size() != rows * cols) {
        throw std::runtime_error("Rows * Cols must equal number of options");
    }
}

void Menu::drawMenu(int prevChoice) {
    int cellWidth = width / cols;
    int startY = y + 3;

    // Draw outer box and header once
    if (prevChoice == -1) {
        AnsiRenderer::invertOn();
        AnsiRenderer::drawBox(x, y, width, height);
        AnsiRenderer::drawText(x + (width - header.size()) / 2, y + 1, header);
        AnsiRenderer::flush();
        AnsiRenderer::invertOff();

        // Draw all options inverted initially
        for (int i = 0; i < (int)options.size(); ++i) {
            int row = i / cols;
            int col = i % cols;
            int itemX = x + col * cellWidth;
            int itemY = startY + row * 3;

            AnsiRenderer::invertOn();
            AnsiRenderer::drawBox(itemX, itemY, cellWidth, 3);
            AnsiRenderer::drawText(itemX + (cellWidth - options[i].size()) / 2,
                                   itemY + 1,
                                   options[i]);
            AnsiRenderer::flush();
            AnsiRenderer::invertOff();
        }
    }

    // Restore previous selected option to inverted
    if (prevChoice >= 0 && prevChoice != currentChoice) {
        int row = prevChoice / cols;
        int col = prevChoice % cols;
        int itemX = x + col * cellWidth;
        int itemY = startY + row * 3;

        AnsiRenderer::invertOn();
        AnsiRenderer::drawBox(itemX, itemY, cellWidth, 3);
        AnsiRenderer::drawText(itemX + (cellWidth - options[prevChoice].size()) / 2,
                               itemY + 1,
                               options[prevChoice]);
        AnsiRenderer::flush();
        AnsiRenderer::invertOff();
    }

    // Draw current selected option as normal (non-inverted)
    int row = currentChoice / cols;
    int col = currentChoice % cols;
    int itemX = x + col * cellWidth;
    int itemY = startY + row * 3;

    AnsiRenderer::drawBox(itemX, itemY, cellWidth, 3);
    AnsiRenderer::drawText(itemX + (cellWidth - options[currentChoice].size()) / 2,
                           itemY + 1,
                           options[currentChoice]);
    AnsiRenderer::flush();
}

int Menu::show(Keyboard& keyboard) {
    AnsiRenderer::clear();
    AnsiRenderer::hideCursor();

    drawMenu(-1);

    while (true) {
        int prevChoice = currentChoice;
        int key = keyboard.readKey();

        switch(key) {
            case KEY_LEFT:
                if (currentChoice % cols > 0) currentChoice--;
                break;
            case KEY_RIGHT:
                if (currentChoice % cols < cols - 1) currentChoice++;
                break;
            case KEY_UP:
                if (currentChoice >= cols) currentChoice -= cols;
                break;
            case KEY_DOWN:
                if (currentChoice + cols < (int)options.size()) currentChoice += cols;
                break;
            case KEY_ENTER:
                AnsiRenderer::showCursor();
                return currentChoice;
        }

        if (prevChoice != currentChoice) {
            drawMenu(prevChoice);  // redraw only affected options
        }
    }
}

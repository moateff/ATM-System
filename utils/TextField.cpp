#include "TextField.hpp"
#include "Keyboard.hpp"

#include <unistd.h>

TextField::TextField(int x, int y,
                     int width, int capacity,
                     Validator v, 
                     bool masked)
    : x(x), y(y), width(width), capacity(capacity), masked(masked),
      buffer(capacity), validate(v) {}

void TextField::redraw() {
    std::string visible =
        masked ? std::string(buffer.text().size(), '*')
               : buffer.text();

    AnsiRenderer::drawText(x, y, std::string(capacity, ' '));
    AnsiRenderer::drawText(x, y, visible);
    AnsiRenderer::move(x + buffer.cursor(), y);
    AnsiRenderer::flush();
}

std::string TextField::read(Keyboard& kb) {
    redraw();

    while (true) {
        int key = kb.readKey();

        switch (key) {
            case KEY_ENTER: return buffer.text();
            case KEY_ESCAPE: return "";

            case KEY_BACKSPACE: buffer.backspace(); break;
            case KEY_DELETE:    buffer.erase(); break;
            case KEY_LEFT:      buffer.moveLeft(); break;
            case KEY_RIGHT:     buffer.moveRight(); break;
            case KEY_HOME:      buffer.moveHome(); break;
            case KEY_END:       buffer.moveEnd(); break;

            default:
                if (key >= 32 && key <= 126 && validate((char)key))
                    buffer.insert((char)key);
        }
        redraw();
    }
}

int TextField::getX() const { return x; }
int TextField::getY() const { return y; }
int TextField::getWidth() const { return width; }
int TextField::getCapacity() const { return capacity; }



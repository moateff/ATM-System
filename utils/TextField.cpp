#include "TextField.hpp"
#include "Keyboard.hpp"

#include <unistd.h>

TextField::TextField(int x, int y,
                     size_t width,
                     Keyboard& kb,
                     Validator v, 
                     bool masked)
    : x(x), y(y), width(width), masked(masked), keyboard(kb),
      buffer(width), validate(v) {}

std::string TextField::read() {
    AnsiRenderer::move(x, y);
    
    while (true) {
        // int key = keyboard.readKey();

        int key = getchar();
        
        switch (key) {
            case KEY_ENTER:
                return buffer.text();

            case KEY_ESCAPE:
                return "";

            case KEY_BACKSPACE:
                buffer.backspace();
                break;

            case KEY_LEFT:
                buffer.moveLeft();
                break;

            case KEY_RIGHT:
                buffer.moveRight();
                break;

            case KEY_HOME:
                buffer.moveHome();
                break;

            case KEY_END:
                buffer.moveEnd();
                break;

            case KEY_DELETE:
                buffer.erase();
                break;

            default:
                if (validate((char)key))
                    buffer.insert((char)key);
        }

        std::string visible =
            masked ? std::string(buffer.text().size(), '*')
                : buffer.text();

        AnsiRenderer::drawText(x, y, visible);
    }
}


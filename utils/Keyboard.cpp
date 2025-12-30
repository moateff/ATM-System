#include "Keyboard.hpp"
#include <unistd.h>

int Keyboard::readKeyRaw() {
    char c = 0;
    read(STDIN_FILENO, &c, 1);
    return c;
}

int Keyboard::readKey() {

    int c = readKeyRaw();

    if (c == 27) {
        char seq[2];
        if (read(STDIN_FILENO, &seq[0], 1) <= 0) return KEY_ESCAPE;
        if (read(STDIN_FILENO, &seq[1], 1) <= 0) return KEY_ESCAPE;

        if (seq[0] == '[') {
            switch (seq[1]) {
                case 'D': return KEY_LEFT;
                case 'C': return KEY_RIGHT;
                case 'A': return KEY_UP;
                case 'B': return KEY_DOWN;
                case 'H': return KEY_HOME;
                case 'F': return KEY_END;
                case '3': { char x; read(STDIN_FILENO, &x, 1); return KEY_DELETE; }
            }
        }
        return KEY_ESCAPE;
    }

    if (c == '\n') return KEY_ENTER;       // Linux ENTER (LF = 10)
    if (c == 127)  return KEY_BACKSPACE;   // Linux BACKSPACE
    return c;
}

Keyboard::Keyboard() {
    tcgetattr(STDIN_FILENO, &oldTerm);
    termios raw = oldTerm;
    raw.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
}

Keyboard::~Keyboard() {
    tcsetattr(STDIN_FILENO, TCSANOW, &oldTerm);
}
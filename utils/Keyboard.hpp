#pragma once
#include <termios.h>
#include <unistd.h>

constexpr int KEY_ENTER       = '\n';
constexpr int KEY_BACKSPACE   = 127;
constexpr int KEY_ESCAPE      = 27;

constexpr int KEY_LEFT        = 1000;
constexpr int KEY_RIGHT       = 1001;
constexpr int KEY_UP          = 1002;
constexpr int KEY_DOWN        = 1003;
constexpr int KEY_HOME        = 1004;
constexpr int KEY_END         = 1005;
constexpr int KEY_DELETE      = 1006;

class Keyboard {
public:
    virtual ~Keyboard() = default;
    virtual int readKey() = 0;
};

class LinuxKeyboard : public Keyboard {
public:
    LinuxKeyboard();
    ~LinuxKeyboard();
    int readKey() override;

private:
    termios oldTerm{};
    int readKeyRaw();
};

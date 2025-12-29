#pragma once
#include "Keyboard.hpp"
#include "TextBuffer.hpp"
#include "AnsiRenderer.hpp"
#include <functional>
#include <string>

class TextField {
public:
    using Validator = std::function<bool(char)>;

    TextField(int x, int y, size_t width,
              Keyboard& kb,
              Validator v,
              bool masked = false);

    std::string read();

private:
    int x, y;
    size_t width;
    bool masked;
    Keyboard& keyboard;
    TextBuffer buffer;
    Validator validate;
};

#pragma once
#include "Keyboard.hpp"
#include "TextBuffer.hpp"
#include "AnsiRenderer.hpp"
#include <functional>
#include <string>

class TextField {
public:
    using Validator = std::function<bool(char)>;

    TextField(int x, int y, 
              int width, int capacity,
              Validator v,
              bool masked = false);

    std::string read(Keyboard& kb);
    
    int getX() const;
    int getY() const;
    int getWidth() const;
    int getCapacity() const;

private:
    int x, y;
    int width, capacity;
    bool masked;
    TextBuffer buffer;
    Validator validate;

    void redraw();
};

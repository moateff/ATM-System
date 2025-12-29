#pragma once
#include <string>

class TextBuffer {
public:
    explicit TextBuffer(size_t capacity);

    bool insert(char c);
    bool backspace();
    bool erase();

    void moveLeft();
    void moveRight();
    void moveHome();
    void moveEnd();

    size_t cursor() const;
    const std::string& text() const;
    size_t capacity() const;
    
private:
    std::string buffer;
    size_t cap;
    size_t cur;
};

#include "TextBuffer.hpp"

TextBuffer::TextBuffer(size_t capacity)
    : cap(capacity), cur(0) {}

bool TextBuffer::insert(char c) {
    if (buffer.size() >= cap) return false;
    buffer.insert(buffer.begin() + cur, c);
    cur++;
    return true;
}

bool TextBuffer::backspace() {
    if (cur == 0) return false;
    buffer.erase(cur - 1, 1);
    cur--;
    return true;
}

bool TextBuffer::erase() {
    if (cur >= buffer.size()) return false;
    buffer.erase(cur, 1);
    return true;
}

void TextBuffer::moveLeft()  { if (cur > 0) cur--; }
void TextBuffer::moveRight() { if (cur < buffer.size()) cur++; }
void TextBuffer::moveHome()  { cur = 0; }
void TextBuffer::moveEnd()   { cur = buffer.size(); }

size_t TextBuffer::cursor() const { return cur; }
const std::string& TextBuffer::text() const { return buffer; }
size_t TextBuffer::capacity() const { return cap; }
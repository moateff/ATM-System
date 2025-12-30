#include "Label.hpp"
#include "AnsiRenderer.hpp"

Label::Label(int x, int y, const std::string& text)
    : x(x), y(y), text(text) {}

void Label::draw(bool inverted) const {
    if (inverted) AnsiRenderer::invertOn();
    AnsiRenderer::drawText(x, y, text);
    if (inverted) AnsiRenderer::invertOff();
}

void Label::setText(const std::string& t) {
    text = t;
}

const std::string& Label::getText() const {
    return text;
}

void Label::setPosition(int newX, int newY) {
    x = newX;
    y = newY;
}

int Label::getX() const {
    return x;
}

int Label::getY() const {
    return y;
}

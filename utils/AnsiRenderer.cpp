#include "AnsiRenderer.hpp"
#include <iostream>

void AnsiRenderer::clear() {
    std::cout << "\033[2J\033[H";
}

void AnsiRenderer::move(int x, int y) {
    std::cout << "\033[" << y << ";" << x << "H";
}

void AnsiRenderer::invertOn() {
    std::cout << "\033[7m";
}

void AnsiRenderer::invertOff() {
    std::cout << "\033[27m";
}

void AnsiRenderer::drawText(int x, int y, const std::string& t) {
    move(x, y);
    std::cout << t;
}

void AnsiRenderer::drawBox(int x, int y, int w, int h) {
    for (int i = 0; i < h; ++i) {
        move(x, y + i);
        for (int j = 0; j < w; ++j)
            std::cout << ' ';
    }
}

void AnsiRenderer::hideCursor() {
    std::cout << "\033[?25l";
}

void AnsiRenderer::showCursor() {
    std::cout << "\033[?25h";
}

void AnsiRenderer::flush() {
    std::cout << std::flush;
}

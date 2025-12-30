#pragma once
#include <string>

class AnsiRenderer {
public:
    static void clear();
    static void move(int x, int y);
    static void invertOn();
    static void invertOff();
    static void drawBox(int x, int y, int w, int h);
    static void drawText(int x, int y, const std::string& text);
    static void hideCursor();
    static void showCursor();
    static void flush();
};


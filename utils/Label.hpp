#pragma once
#include <string>

class Label {
public:
    Label(int x, int y, const std::string& text);

    void draw(bool inverted = true) const;

    void setText(const std::string& t);
    const std::string& getText() const;

    void setPosition(int newX, int newY);
    int getX() const;
    int getY() const;

private:
    int x, y;
    std::string text;
};

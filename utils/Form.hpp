#pragma once
#include <vector>
#include <string>
#include "TextField.hpp"
#include "Keyboard.hpp"
#include "Label.hpp"

class Form {
public:
    struct Field {
        Label label;      // Position and text of the label
        TextField field;  // Input field
    };

    Form(int x, int y, int width, int height,
         const std::string& title,
         const std::string& footer = "");

    void addField(Label&& label, TextField&& field);
    
    // Runs the form and returns all field values in order
    std::vector<std::string> run(Keyboard& keyboard);

private:
    int x, y, width, height;
    std::string title;
    std::string footer;

    std::vector<Field> fields;

    void drawFrame();
    void drawLabels();
    void drawFields();
};

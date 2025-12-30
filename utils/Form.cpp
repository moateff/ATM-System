#include "Form.hpp"
#include "AnsiRenderer.hpp"

Form::Form(int x, int y, int width, int height,
           const std::string& title,
           const std::string& footer)
    : x(x), y(y), width(width), height(height),
      title(title), footer(footer) {}

void Form::addField(Label&& label, TextField&& field) {
    fields.push_back({ std::move(label), std::move(field) });
}

void Form::drawFrame() {
    AnsiRenderer::invertOn();
    AnsiRenderer::drawBox(x, y, width, height);

    // Header
    AnsiRenderer::drawText(
        x + (width - title.size()) / 2,
        y + 2,
        title
    );

    // Footer
    AnsiRenderer::drawText(
        x + (width - footer.size()) / 2,
        y + height - 3,
        footer
    );

    AnsiRenderer::invertOff();
}

void Form::drawLabels() {
    for (auto& f : fields) {
        AnsiRenderer::invertOn();
        AnsiRenderer::drawText(f.label.getX(), f.label.getY(), f.label.getText());
        AnsiRenderer::invertOff();
    }
}

void Form::drawFields() {
    for (auto& f : fields) {
        // Draw only the box for the input field
        AnsiRenderer::drawBox(f.field.getX(), f.field.getY(), f.field.getWidth(), 1);
    }
}

std::vector<std::string> Form::run(Keyboard& keyboard) {
    AnsiRenderer::clear();
    AnsiRenderer::hideCursor();

    drawFrame();
    drawLabels();
    drawFields();

    AnsiRenderer::flush();
    AnsiRenderer::showCursor();
    
    std::vector<std::string> results;

    for (auto& f : fields) {
        results.push_back(f.field.read(keyboard));
    }

    return results;
}

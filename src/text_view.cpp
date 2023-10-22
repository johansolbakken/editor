#include "text_view.h"

#include <raylib.h>

void TextView::render() {
    Rectangle scissors = {x, y, width, height};
    BeginScissorMode(scissors.x, scissors.y, scissors.width, scissors.height);

    std::string joined_text = "";
    for (auto line : text) {
        joined_text += line + "\n";
    }
    DrawText(joined_text.c_str(), x, y, 20, (Color){255,255,255,255});

    // end scissors
    EndScissorMode();
}

void TextView::update() {
}
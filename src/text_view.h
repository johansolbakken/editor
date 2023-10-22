#pragma once

#include <vector>
#include <string>

class TextView {
public:
    void render();
    void update();

    std::vector<std::string> text;
    float x = 0;
    float y = 0;
    float width = 0;
    float height = 0;
};
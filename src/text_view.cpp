#include "text_view.h"

#include <raylib.h>

void TextView::render()
{
    Rectangle scissors = {m_x, m_y, m_width, m_height};
    BeginScissorMode(static_cast<int>(scissors.x), static_cast<int>(scissors.y), static_cast<int>(scissors.width), static_cast<int>(scissors.height));

    std::string joined_text = text();
    
    DrawText(joined_text.c_str(), static_cast<int>(m_x), static_cast<int>(m_y), 20, (Color){255, 255, 255, 255});

    // end scissors
    EndScissorMode();
}

void TextView::update()
{
}

std::string TextView::text() const
{
    std::string joined_text = "";
    for (auto line : m_text)
    {
        joined_text += line + "\n";
    }
    return joined_text;
}
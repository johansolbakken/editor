#include "text_view.h"

#include <raylib.h>

TextView::TextView()
{
	m_text.emplace_back("");
}

void TextView::render(Font font)
{
    Rectangle scissors = {m_x, m_y, m_width, m_height};
    BeginScissorMode(static_cast<int>(scissors.x), static_cast<int>(scissors.y), static_cast<int>(scissors.width), static_cast<int>(scissors.height));

    std::string joined_text = text();

	Vector2 position = {m_x, m_y};
	DrawTextEx(font, reinterpret_cast<const char*>(joined_text.c_str()), position, 20, 1, WHITE);

    // end scissors
    EndScissorMode();
}

void TextView::update()
{
}

std::string TextView::text() const
{
    std::string joined_text;
    for (const auto& line : m_text)
    {
        joined_text += line;
		joined_text += "\n";
    }
    return joined_text;
}

void TextView::insert_char(wchar_t c)
{
	m_text[m_line].insert(m_cursor, 1, c);
	m_cursor++;
}

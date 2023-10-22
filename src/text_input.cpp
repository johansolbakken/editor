#include "text_input.h"

TextInput::TextInput()
{

}

void TextInput::render()
{
	BeginScissorMode(static_cast<int>(m_x), static_cast<int>(m_y), static_cast<int>(m_width), static_cast<int>(m_height));

	auto background_color = DARKGRAY;
	DrawRectangleRounded({m_x, m_y, m_width, m_height}, 0.4f, 0, background_color);

	float font_height = 20;
	float font_spacing = 2;

	// center the text
	float text_x = m_x + 8;
	float text_y = m_y + m_height / 2 - font_height / 2;

	DrawTextEx(GetFontDefault(), m_text.c_str(), {text_x - m_scroll_x, text_y}, font_height, font_spacing, WHITE);

	EndScissorMode();
}

void TextInput::update()
{
	if (m_scroll_x < 0) m_scroll_x = 0;
}

void TextInput::insert_char(wchar_t c)
{
	m_text.insert(m_cursor, 1, c);
	m_cursor++;
}

void TextInput::delete_left_char()
{
	if (m_cursor == 0) return;
	m_text.erase(m_cursor - 1, 1);
	m_cursor--;
}

void TextInput::move_cursor_left()
{
	m_cursor--;
	if (m_cursor < 0) m_cursor = 0;
}

void TextInput::move_cursor_right()
{
	m_cursor++;
	if (m_cursor > m_text.size()) m_cursor = m_text.size();
}

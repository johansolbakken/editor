#include "text_input.h"

extern Font font;

TextInput::TextInput()
{

}

void TextInput::render()
{
	if (!m_visible) return;
	BeginScissorMode(static_cast<int>(m_x), static_cast<int>(m_y), static_cast<int>(m_width), static_cast<int>(m_height));

	auto background_color = (Color){25,25,25,255};
	DrawRectangleRounded({m_x, m_y, m_width, m_height}, 0.4f, 0, background_color);

	float font_height = 20;
	float font_spacing = 1;

	// center the text
	float text_x = m_x + 8;
	float text_y = m_y + m_height / 2 - font_height / 2;

	DrawTextEx(font, m_text.c_str(), {text_x - m_scroll_x, text_y}, font_height, font_spacing, WHITE);

	float rate = 0.2f;
	float state = static_cast<float>(GetTime()) / rate;
	if (!m_focused || sin(state) > 0) {
		EndScissorMode();
		return;
	}

	// cursor as "|"
	float cursor_x = MeasureTextEx(font, m_text.substr(0,m_cursor).c_str(), font_height, font_spacing).x
					- MeasureTextEx(font, "|", font_height, font_spacing).x / 2;
	DrawTextEx(font, "|", {text_x + cursor_x - m_scroll_x, text_y}, font_height, font_spacing, GRAY);

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

std::string TextInput::text() const
{
	return m_text;
}

void TextInput::cursor_to_end()
{
	m_cursor = m_text.size();
}

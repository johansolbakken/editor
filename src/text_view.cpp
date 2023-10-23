#include "text_view.h"

#include <raylib.h>

extern Font font;

TextView::TextView()
{
	m_text.emplace_back("");

	m_scroll_x = -100;
	m_scroll_y = -100;
}

void TextView::render()
{
	float font_height = 20;
	float font_spacing = 1;
	float line_spacing = 2;

    Rectangle scissors = {m_x, m_y, m_width, m_height};
    BeginScissorMode(static_cast<int>(scissors.x), static_cast<int>(scissors.y), static_cast<int>(scissors.width), static_cast<int>(scissors.height));

	float x = 0;
	float y = 0;
	for (const auto& line : m_text) {
		DrawTextEx(font, line.c_str(), {m_x + x - m_scroll_x, m_y + y - m_scroll_y}, font_height, font_spacing, WHITE);
		y += font_height + line_spacing;
	}

	float rate = 0.2f;
	float state = static_cast<float>(GetTime()) / rate;
	if (!m_focused || sin(state) > 0) {
		EndScissorMode();
		return;
	}

	// cursor as "|"
	float cursor_x = MeasureTextEx(font, m_text[m_line].substr(0,m_cursor).c_str(), font_height, font_spacing).x 
					- MeasureTextEx(font, "|", font_height, font_spacing).x / 2;
	float cursor_y = m_line * (font_height + line_spacing);
	DrawTextEx(font, "|", {m_x + cursor_x - m_scroll_x, m_y + cursor_y - m_scroll_y}, font_height, font_spacing, BLUE);

    // end scissors
    EndScissorMode();
}

void TextView::update()
{
}

std::string TextView::text() const
{
    std::string joined_text;
	int i = 0;
    for (const auto& line : m_text)
    {
        joined_text += line;

		if (i < m_text.size() - 1) joined_text += "\n";
		i++;
    }
    return joined_text;
}

void TextView::insert_char(wchar_t c)
{
	if (!m_focused) return;
	m_text[m_line].insert(m_cursor, 1, c);
	m_cursor++;
}

void TextView::insert_enter()
{
	if (!m_focused) return;

	if (m_line == m_text.size() - 1) {
		m_text.emplace_back("");
		m_line++;
		m_cursor = 0;
		return;
	}

	if (m_cursor == m_text[m_line].size()) {
		m_text.emplace(m_text.begin() + m_line + 1, "");
		m_line++;
		m_cursor = 0;
		return;
	}

	std::string before_cursor;
	std::string new_line;

	for (int i = 0; i < m_text[m_line].size(); i++) {
		if (i < m_cursor) {
			before_cursor.push_back(m_text[m_line][i]);
		} else {
			new_line.push_back(m_text[m_line][i]);
		}
	}

	m_text[m_line] = before_cursor;
	m_text.emplace(m_text.begin() + m_line + 1, new_line);
	m_line++;
	m_cursor = 0;
}

void TextView::delete_left_char()
{
	if (!m_focused) return;
	if (m_line == 0 && m_cursor == 0) {
		return;
	}

	if (m_cursor == 0) {
		std::string new_line;
		int cursor_pos = m_text[m_line - 1].size();
		for (int i = 0; i < m_text[m_line - 1].size(); i++) {
			new_line.push_back(m_text[m_line - 1][i]);
		}
		for (int i = 0; i < m_text[m_line].size(); i++) {
			new_line.push_back(m_text[m_line][i]);
		}
		m_text[m_line - 1] = new_line;
		m_text.erase(m_text.begin() + m_line);
		m_line--;
		m_cursor = cursor_pos;
		return;
	}

	std::string new_line;
	for (int i = 0; i < m_text[m_line].size(); i++) {
		if (i == m_cursor - 1) {
			continue;
		}
		new_line.push_back(m_text[m_line][i]);
	}

	m_text[m_line] = new_line;
	m_cursor--;
}

void TextView::move_cursor_left()
{
	if (!m_focused) return;
	if (m_cursor == 0) {
		if (m_line == 0) {
			return;
		}
		m_line--;
		m_cursor = m_text[m_line].size();
		return;
	}

	m_cursor--;
	if (m_cursor < 0) {
		m_cursor = 0;
	}
}

void TextView::move_cursor_right()
{
	if (!m_focused) return;
	if (m_cursor == m_text[m_line].size()) {
		if (m_line == m_text.size() - 1) {
			return;
		}
		m_line++;
		m_cursor = 0;
		return;
	}

	m_cursor++;
	if (m_cursor > m_text[m_line].size()) {
		m_cursor = m_text[m_line].size();
	}
}

void TextView::move_cursor_up()
{
	if (!m_focused) return;
	if (m_line == 0) {
		return;
	}
	m_line--;
	if (m_cursor > m_text[m_line].size()) {
		m_cursor = m_text[m_line].size();
	}
}

void TextView::move_cursor_down()
{
	if (!m_focused) return;
	if (m_line == m_text.size() - 1) {
		return;
	}
	m_line++;
	if (m_cursor > m_text[m_line].size()) {
		m_cursor = m_text[m_line].size();
	}
}

void TextView::set_text(const std::string& text)
{
	std::vector<std::string> lines;
	std::string line;
	for (const auto& c : text) {
		if (c == '\n') {
			lines.emplace_back(line);
			line.clear();
			continue;
		}
		line.push_back(c);
	}
	m_text = lines;
}

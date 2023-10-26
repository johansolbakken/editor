#include "text_view.h"

#include <raylib.h>

#include "highlight.h"

extern Font font;

TextView::TextView()
{
	m_text.emplace_back("");

	m_scroll_x = -100;
	m_scroll_y = -100;
}

void TextView::render()
{
    Rectangle scissors = {m_x, m_y, m_width, m_height};
    BeginScissorMode(static_cast<int>(scissors.x), static_cast<int>(scissors.y), static_cast<int>(scissors.width), static_cast<int>(scissors.height));

	auto tokens = get_colors(m_text);

	float y = 0;
	for (const auto& token : tokens) {
		float x = 0;

		for (auto t : token) {
			if (t.text == "\t") t.text = "    ";
			float word_width = MeasureTextEx(font, t.text.c_str(), m_font_height, m_font_spacing).x;
			DrawTextEx(font, t.text.c_str(), {m_x + x - m_scroll_x, m_y + y - m_scroll_y}, m_font_height, m_font_spacing, t.color);
			x += word_width + m_font_spacing;
		}

		y += m_font_height + m_line_spacing;
	}

	float rate = 0.2f;
	float state = static_cast<float>(GetTime()) / rate;
	if (!m_focused || sin(state) > 0) {
		EndScissorMode();
		return;
	}

	// cursor as "|"
	std::string current_line;
	for (const auto& c : m_text[m_line]) {
		if (c == '\t') {
			current_line += "    ";
		} else {
			current_line += c;
		}
	}
	float cursor_x = MeasureTextEx(font, current_line.substr(0,m_cursor).c_str(), m_font_height, m_font_spacing).x
					- MeasureTextEx(font, "|", m_font_height, m_font_spacing).x / 2;
	float cursor_y = m_line * (m_font_height + m_line_spacing);
	DrawTextEx(font, "|", {m_x + cursor_x - m_scroll_x, m_y + cursor_y - m_scroll_y}, m_font_height, m_font_spacing, BLUE);

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

	if (m_line == m_text.size() - 1 && m_cursor == m_text[m_line].size()) {
		int space_count = 0;
		for (const auto& c : m_text[m_line]) {
			if (c == ' ') {
				space_count++;
			} else {
				break;
			}
		}
		m_text.emplace_back("");
		for (int i = 0; i < space_count; i++) {
			m_text[m_line + 1].push_back(' ');
		}
		m_line++;
		m_cursor = space_count;
		return;
	}

	if (m_cursor == m_text[m_line].size()) {
		int space_count = 0;
		for (const auto& c : m_text[m_line + 1]) {
			if (c == ' ') {
				space_count++;
			} else {
				break;
			}
		}
		m_text.emplace(m_text.begin() + m_line + 1, "");
		for (int i = 0; i < space_count; i++) {
			m_text[m_line + 1].push_back(' ');
		}
		m_line++;
		m_cursor = space_count;
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

	int before_cursor_space_count = 0;
	for (const auto& c : before_cursor) {
		if (c == ' ') {
			before_cursor_space_count++;
		} else {
			break;
		}
	}

	new_line.insert(0, before_cursor.substr(0, before_cursor_space_count));

	m_text[m_line] = before_cursor;
	m_text.insert(m_text.begin() + m_line + 1, new_line);
	m_line++;
	m_cursor = before_cursor_space_count;
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

void TextView::mouse_click(int x, int y)
{
	if (!m_focused) return;

	float font_height = m_font_height;
	float font_spacing = m_font_spacing;
	float line_spacing = m_line_spacing;

	float text_x = m_x - m_scroll_x;
	float text_y = m_y - m_scroll_y;

	int line = 0;
	int cursor = 0;

	// Calculate which line was clicked
	line = static_cast<int>((y - text_y) / (font_height + line_spacing));

	// If we have the lines of text stored in an array/vector
	// Ensure that the click is within the bounds of our text
	if (line < 0) line = 0;
	if (line >= m_text.size()) line = m_text.size() - 1;

	// Calculate cursor position within the line for variable-width font
	float accumulatedWidth = 0;
	for (int i = 0; i < m_text[line].length(); i++) {
		float charWidth = MeasureTextEx(font, m_text[line].substr(i, 1).c_str(), m_font_height, m_font_spacing).x;

		// Compare including the spacing
		if (x - text_x < accumulatedWidth + charWidth) {
			cursor = i;
			break;
		}

		accumulatedWidth += charWidth;
	}
	if (accumulatedWidth < (x - text_x)) { // If the click was beyond the end of the line
		cursor = m_text[line].length();
	}

	m_line = line;
	m_cursor = cursor;
}

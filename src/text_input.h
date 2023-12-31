#pragma once

#include <string>

#include <raylib.h>

class TextInput {
public:
	TextInput();
	void render();
	void update();

	[[nodiscard]] std::string text() const;
	void set_text(const std::string& text) { m_text = text; }

	void set_width(float width) { m_width = width; }
	void set_height(float height) { m_height = height; }

	void insert_char(wchar_t c);
	void delete_left_char();

	void move_cursor_left();
	void move_cursor_right();
	void cursor_to_end();

	void set_focused(bool focused) { m_focused = focused; }
	bool focused() const { return m_focused; }

	void set_visible(bool visible) { m_visible = visible; }
	bool visible() const { return m_visible; }

	void scroll_horizontal(float delta) { m_scroll_x += delta; }

	[[nodiscard]] Rectangle bounds() const { return {m_x, m_y, m_width, m_height}; }

	void set_x(float x) { m_x = x; }
	void set_y(float y) { m_y = y; }

private:
	float m_x = 0;
	float m_y = 0;
	float m_width = 0;
	float m_height = 0;

	int m_cursor = 0;
	float m_scroll_x = 0;
	bool m_focused = false;
	bool m_visible = false;

	std::string m_text;
};
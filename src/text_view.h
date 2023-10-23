#pragma once

#include <vector>
#include <string>

#include <raylib.h>

class TextView {
public:
    TextView();
    void render();
    void update();

    [[nodiscard]] std::string text() const;
	void set_text(const std::string& text);

    void set_width(float width) { m_width = width; }
    void set_height(float height) { m_height = height; }

	void insert_char(wchar_t c);
	void insert_enter();
	void delete_left_char();

	void move_cursor_left();
	void move_cursor_right();
	void move_cursor_up();
	void move_cursor_down();

	void set_focused(bool focused) { m_focused = focused; }
	bool focused() const { return m_focused; }

	void scroll_horizontal(float delta) { m_scroll_x += delta; }
	void scroll_vertical(float delta) { m_scroll_y += delta; }

	[[nodiscard]] Rectangle bounds() const { return {m_x, m_y, m_width, m_height}; }

	void mouse_click(int x, int y);

private:
    std::vector<std::string> m_text;
    float m_x = 0;
    float m_y = 0;
    float m_width = 0;
    float m_height = 0;

    int m_cursor = 0;
    int m_line = 0;

	bool m_focused = false;

	float m_scroll_x = 0;
	float m_scroll_y = 0;
};
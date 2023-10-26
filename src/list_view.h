#pragma once

#include <string>
#include <vector>

#include <raylib.h>

class ListView
{
public:
	ListView();

	void render();

	void update();

	void set_x(float x)
	{
		m_x = x;
	}

	void set_y(float y)
	{
		m_y = y;
	}

	void set_width(float width)
	{
		m_width = width;
	}

	void set_height(float height)
	{
		m_height = height;
	}

	void set_items(const std::vector<std::string>& items)
	{
		m_items = items;
	}

	[[nodiscard]] std::string selected_item() const;

	Rectangle bounds() const
	{
		return {m_x, m_y, m_width, m_height};
	}

	void move_cursor_up()
	{
		m_selected_item--;
		if (m_selected_item < 0) {
			m_selected_item = 0;
		}
	}

	void move_cursor_down()
	{
		m_selected_item++;
		if (m_selected_item >= m_items.size()) {
			m_selected_item = m_items.size() - 1;
		}
	}

	const std::vector<std::string>& items() const
	{
		return m_items;
	}

	void set_selected_item(int selected_item)
	{
		m_selected_item = selected_item;
	}

	void scroll_horizontal(float amount)
	{
		m_scroll_x += amount;
		if (m_scroll_x < 0) {
			m_scroll_x = 0;
		}
	}

	void scroll_vertical(float amount)
	{
		m_scroll_y += amount;
		if (m_scroll_y < 0) {
			m_scroll_y = 0;
		}
	}

private:
	float m_x = 0;
	float m_y = 0;
	float m_width = 0;
	float m_height = 0;
	float m_scroll_x = 0;
	float m_scroll_y = 0;

	std::vector<std::string> m_items;
	int m_selected_item = 0;
};
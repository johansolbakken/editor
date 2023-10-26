#include "list_view.h"

#include <raylib.h>

extern Font font;

ListView::ListView()
{
}

void ListView::render()
{
	BeginScissorMode(m_x, m_y, m_width, m_height);

	// gray background
	DrawRectangle(m_x, m_y, m_width, m_height, (Color){25,25,25,255});

	int padding = 8;

	int y = 0;
	for (const auto& item : m_items) {
		if (m_selected_item == y / 20) {
			DrawRectangle(m_x, m_y + y - m_scroll_y, m_width, 20, (Color){88,88,88,255});
		}
		std::string file_name = item.substr(item.find_last_of("/\\") + 1);
		DrawTextEx(font, file_name.c_str(), {padding + m_x - m_scroll_x, m_y + y - m_scroll_y}, 20, 1, WHITE);
		
		int file_name_width = MeasureTextEx(font, (file_name + "  ").c_str(), 20, 1).x;
		float delta_height = (20 - 15) / 2;
		DrawTextEx(font, item.c_str(), {padding + m_x + file_name_width - m_scroll_x, m_y + y - m_scroll_y + delta_height}, 15, 1, LIGHTGRAY);
		y += 20;
	}

	EndScissorMode();
}

void ListView::update()
{

}

std::string ListView::selected_item() const
{
	if (m_selected_item < 0 || m_selected_item >= m_items.size()) {
		return "";
	}

	return m_items[m_selected_item];
}

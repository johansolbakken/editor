#include "application.h"

#include <raylib.h>

void App::input()
{
	if (WindowShouldClose() && !IsKeyPressed(KEY_ESCAPE)) {
		m_running = false;
		return;
	}

	int unicode = GetCharPressed();
	if (unicode > 0) m_text_view.insert_char(unicode);
	if (IsKeyPressed(KEY_ENTER)) m_text_view.insert_enter();
	if (IsKeyPressed(KEY_BACKSPACE)) m_text_view.delete_left_char();

	if (IsKeyPressed(KEY_LEFT)) m_text_view.move_cursor_left();
	if (IsKeyPressed(KEY_RIGHT)) m_text_view.move_cursor_right();
	if (IsKeyPressed(KEY_UP)) m_text_view.move_cursor_up();
	if (IsKeyPressed(KEY_DOWN)) m_text_view.move_cursor_down();

	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), m_text_view.bounds())) m_text_view.set_focused(true);
	if (IsKeyPressed(KEY_ESCAPE)) m_text_view.set_focused(false);

	float scroll_speed = 4;
	auto mouse_scroll = GetMouseWheelMoveV();
	m_text_view.scroll_vertical(-mouse_scroll.y * scroll_speed);
	m_text_view.scroll_horizontal(-mouse_scroll.x * scroll_speed);

	m_text_view.set_width(GetScreenWidth());
	m_text_view.set_height(GetScreenHeight());
}

void App::run() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(800, 600, "Hello World");

    m_text_view.set_width(800);
    m_text_view.set_height(600);

    while (m_running) {
		input();
        BeginDrawing();
        ClearBackground((Color){0,0,0,255});
        m_text_view.render();
        EndDrawing();
    }
    CloseWindow();
}

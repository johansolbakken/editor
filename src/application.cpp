#include "application.h"

#include <raylib.h>

App::App()
{
	m_text_view.set_width(800);
	m_text_view.set_height(600);

	m_text_input.set_width(600);
	m_text_input.set_height(40);
	m_text_input.set_x(GetScreenWidth() / 2 - m_text_input.bounds().width / 2);
	m_text_input.set_y(40);
}

void App::run() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(800, 600, "Hello World");

    while (m_running) {
		input();
		update();
		render();
    }

    CloseWindow();
}

void App::input()
{
	if (WindowShouldClose() && !IsKeyPressed(KEY_ESCAPE)) {
		m_running = false;
		return;
	}

	if (IsKeyDown(KEY_LEFT_SUPER) && IsKeyPressed(KEY_P)) {
		m_text_view.set_focused(false);
		m_text_input.set_visible(true);
		m_text_input.set_focused(true);
	}

	int unicode = GetCharPressed();
	if (unicode > 0)
	{
		if (m_text_view.focused()) m_text_view.insert_char(unicode);
		if (m_text_input.focused()) m_text_input.insert_char(unicode);
	}

	if (m_text_view.focused())
	{
		if (IsKeyPressed(KEY_ENTER)) m_text_view.insert_enter();
		if (IsKeyPressed(KEY_BACKSPACE)) m_text_view.delete_left_char();

		if (IsKeyPressed(KEY_LEFT)) m_text_view.move_cursor_left();
		if (IsKeyPressed(KEY_RIGHT)) m_text_view.move_cursor_right();
		if (IsKeyPressed(KEY_UP)) m_text_view.move_cursor_up();
		if (IsKeyPressed(KEY_DOWN)) m_text_view.move_cursor_down();
	}

	if (m_text_input.focused())
	{
		if (IsKeyPressed(KEY_BACKSPACE)) m_text_input.delete_left_char();

		if (IsKeyPressed(KEY_LEFT)) m_text_input.move_cursor_left();
		if (IsKeyPressed(KEY_RIGHT)) m_text_input.move_cursor_right();
	}

	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), m_text_input.bounds())) {
		m_text_input.set_focused(true);
		m_text_view.set_focused(false);
	}
	else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), m_text_view.bounds())) {
		m_text_view.set_focused(true);
		m_text_input.set_focused(false);
		m_text_input.set_visible(false);
	}

	if (IsKeyPressed(KEY_ESCAPE)) {
		m_text_view.set_focused(false);
		m_text_input.set_focused(false);
		m_text_input.set_visible(false);
	}

	float scroll_speed = 4;
	auto mouse_scroll = GetMouseWheelMoveV();
	if (m_text_input.focused()) {
		m_text_input.scroll_horizontal(-mouse_scroll.x * scroll_speed);
	}

	if (m_text_view.focused()) {
		m_text_view.scroll_vertical(-mouse_scroll.y * scroll_speed);
		m_text_view.scroll_horizontal(-mouse_scroll.x * scroll_speed);
	}

	m_text_view.set_width(GetScreenWidth());
	m_text_view.set_height(GetScreenHeight());
}

void App::update()
{
	m_text_view.update();
	m_text_input.update();
	m_text_input.set_x(GetScreenWidth() / 2 - m_text_input.bounds().width / 2);
}

void App::render()
{
	BeginDrawing();
	ClearBackground((Color){0,0,0,255});
	m_text_view.render();
	m_text_input.render();
	EndDrawing();
}



void App::new_file()
{

}

void App::open_file()
{

}

void App::save_file()
{

}

void App::save_file_as()
{

}



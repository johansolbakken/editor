#include "application.h"

#include <raylib.h>

#include <fstream>

Font font;

App::App()
{
	m_text_view.set_width(800);
	m_text_view.set_height(600);

	m_text_input.set_width(600);
	m_text_input.set_height(40);
	m_text_input.set_x(GetScreenWidth() / 2 - m_text_input.bounds().width / 2);
	m_text_input.set_y(40);

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);

	InitWindow(800, 600, "Hello World");

	font = LoadFontEx("assets/fonts/Cascadia.ttf", 40, 0, 0);
}

App::~App()
{
	UnloadFont(font);
	CloseWindow();
}

void App::run()
{
	while (m_running)
	{
		input();
		update();
		render();
	}
}

void App::input()
{
	if (WindowShouldClose() && !IsKeyPressed(KEY_ESCAPE))
	{
		m_running = false;
		return;
	}

	if (IsKeyDown(KEY_LEFT_SUPER))
	{
		if (IsKeyPressed(KEY_P))
		{
			m_text_view.set_focused(false);
			m_text_input.set_visible(true);
			m_text_input.set_focused(true);
			m_text_input.set_text("");
			m_text_input.cursor_to_end();
		}
		if (IsKeyPressed(KEY_S))
		{
			save_file(m_file_path);
		}
		if (IsKeyPressed(KEY_O))
		{
			m_text_view.set_focused(false);
			m_text_input.set_visible(true);
			m_text_input.set_focused(true);
			m_text_input.set_text("open ");
			m_text_input.cursor_to_end();
		}
		if (IsKeyPressed(KEY_N))
		{
			new_file();
		}
		if (IsKeyDown(KEY_LEFT_SHIFT) && IsKeyPressed(KEY_S))
		{
			m_text_view.set_focused(false);
			m_text_input.set_visible(true);
			m_text_input.set_focused(true);
			m_text_input.set_text("save ");
			m_text_input.cursor_to_end();
		}
	}

	int unicode = GetCharPressed();
	if (unicode > 0)
	{
		if (m_text_view.focused()) m_text_view.insert_char(unicode);
		if (m_text_input.focused()) m_text_input.insert_char(unicode);
	}

	if (m_text_view.focused())
	{
		if (IsKeyPressed(KEY_ENTER) || IsKeyPressedRepeat(KEY_ENTER)) m_text_view.insert_enter();
		if (IsKeyPressed(KEY_BACKSPACE) || IsKeyPressedRepeat(KEY_BACKSPACE)) m_text_view.delete_left_char();
		if (IsKeyPressed(KEY_TAB)) {
			m_text_view.insert_char(' ');
			m_text_view.insert_char(' ');
			m_text_view.insert_char(' ');
			m_text_view.insert_char(' ');
		}

		if (IsKeyPressed(KEY_LEFT) || IsKeyPressedRepeat(KEY_LEFT)) m_text_view.move_cursor_left();
		if (IsKeyPressed(KEY_RIGHT) || IsKeyPressedRepeat(KEY_RIGHT)) m_text_view.move_cursor_right();
		if (IsKeyPressed(KEY_UP) || IsKeyPressedRepeat(KEY_UP)) m_text_view.move_cursor_up();
		if (IsKeyPressed(KEY_DOWN) || IsKeyPressedRepeat(KEY_DOWN)) m_text_view.move_cursor_down();
	}

	if (m_text_input.focused())
	{
		if (IsKeyPressed(KEY_BACKSPACE)) m_text_input.delete_left_char();

		if (IsKeyPressed(KEY_LEFT) || IsKeyPressedRepeat(KEY_LEFT)) m_text_input.move_cursor_left();
		if (IsKeyPressed(KEY_RIGHT) || IsKeyPressedRepeat(KEY_RIGHT)) m_text_input.move_cursor_right();

		if (IsKeyPressed(KEY_ENTER)) parse_command();
	}

	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), m_text_input.bounds()))
	{
		m_text_input.set_focused(true);
		m_text_view.set_focused(false);
	}
	else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
			 CheckCollisionPointRec(GetMousePosition(), m_text_view.bounds()))
	{
		m_text_view.set_focused(true);
		m_text_input.set_focused(false);
		m_text_input.set_visible(false);
		m_text_view.mouse_click(GetMouseX(), GetMouseY());
	}

	if (IsKeyPressed(KEY_ESCAPE))
	{
		m_text_view.set_focused(false);
		m_text_input.set_focused(false);
		m_text_input.set_visible(false);
	}

	float scroll_speed = 4;
	auto mouse_scroll = GetMouseWheelMoveV();
	if (m_text_input.focused())
	{
		m_text_input.scroll_horizontal(-mouse_scroll.x * scroll_speed);
	}

	if (m_text_view.focused())
	{
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
	SetWindowTitle(("Johan Editor - " + m_file_path).c_str());
}

void App::render()
{
	BeginDrawing();
	ClearBackground((Color){ 0, 0, 0, 255 });
	m_text_view.render();
	m_text_input.render();
	EndDrawing();
}

void App::parse_command()
{
	std::string command = m_text_input.text();
	command.erase(0, command.find_first_not_of(' '));
	command.erase(command.find_last_not_of(' ') + 1);

	if (command == "quit" || command == "exit")
	{
		m_running = false;
		return;
	}

	if (command.starts_with("open") && command.size() > 5)
	{
		std::string file_path = command.substr(5);
		open_file(file_path);
		return;
	}

	if (command.starts_with("save") && command.size() > 5)
	{
		std::string file_path = command.substr(5);
		save_file(file_path);
		return;
	}

	if (command == "new")
	{
		new_file();
		return;
	}
}

void App::open_file(const std::string& file_path)
{
	if (file_path.empty()) return;
	m_file_path = file_path;

	std::ifstream file(file_path);

	if (!file.is_open()) return;

	std::string text;
	std::string line;
	while (std::getline(file, line))
	{
		text += line + "\n";
	}

	m_text_view.set_text(text);

	m_text_input.set_visible(false);
	m_text_input.set_focused(false);
	m_text_view.set_focused(true);
}

void App::save_file(const std::string& file_path)
{
	std::string path = file_path.empty() ? m_file_path : file_path;
	if (path.empty()) return;

	std::ofstream file(path);
	std::string text = m_text_view.text();
	file << text;
	file.close();

	m_file_path = path;

	m_text_input.set_visible(false);
	m_text_input.set_focused(false);
}

void App::new_file()
{
	m_file_path = "";

	m_text_view.set_text("");
	m_text_view.set_focused(true);
	m_text_input.set_visible(false);
	m_text_input.set_focused(false);
}

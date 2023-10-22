#include "application.h"

#include <raylib.h>

#include "text_view.h"

void App::run() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(800, 600, "Hello World");

    TextView text_view;
    text_view.set_width(800);
    text_view.set_height(600);

    while (!WindowShouldClose()) {
		int unicode = GetCharPressed();
		if (unicode > 0) text_view.insert_char(unicode);
		if (IsKeyPressed(KEY_ENTER)) text_view.insert_enter();
		if (IsKeyPressed(KEY_BACKSPACE)) text_view.delete_left_char();

		if (IsKeyPressed(KEY_LEFT)) text_view.move_cursor_left();
		if (IsKeyPressed(KEY_RIGHT)) text_view.move_cursor_right();
		if (IsKeyPressed(KEY_UP)) text_view.move_cursor_up();
		if (IsKeyPressed(KEY_DOWN)) text_view.move_cursor_down();

        BeginDrawing();
        ClearBackground((Color){0,0,0,255});
        text_view.render();
        EndDrawing();
    }
    CloseWindow();
}

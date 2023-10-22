#include "application.h"

#include <raylib.h>

#include "text_view.h"

void App::run() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(800, 600, "Hello World");

	Font font = LoadFontEx("src/font/Cascadia.ttf", 50, nullptr, 0);

    TextView text_view;
    text_view.set_width(800);
    text_view.set_height(600);

	text_view.insert_char('a');
	text_view.insert_char('b');
	text_view.insert_char(u'ø');

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground((Color){0,0,0,255});
        text_view.render(font);
        EndDrawing();
    }
    CloseWindow();
}

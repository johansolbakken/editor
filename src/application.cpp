#include "application.h"

#include <raylib.h>

#include "text_view.h"

void App::run() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(800, 600, "Hello World");

    TextView text_view;
    text_view.width = 800;
    text_view.height = 600;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground((Color){0,0,0,255});
        text_view.render();
        EndDrawing();
    }
    CloseWindow();
}

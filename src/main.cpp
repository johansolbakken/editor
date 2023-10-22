#include <raylib.h>

int main(void) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(800, 600, "Hello World");

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground((Color){0,0,0,255});
        DrawText("Hello World", 10, 10, 20, (Color){255,255,255,255});
        EndDrawing();
    }
    CloseWindow();
    return 0;
}

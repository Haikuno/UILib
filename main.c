#include "src/ui_widget.h"
#include <raylib.h>

int main(int argc, char *argv[]) {
    auto widget   = UIWidget_create();
    auto widget_2 = UIWidget_create("x", 320.0f, "r", 255, "a", 128);
    // init raylib
    InitWindow(640, 480, "UI Widget Example");
    SetTargetFPS(60);

    // draw it
    while (true) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        UIWidget_draw(widget);
        UIWidget_draw(widget_2);
        EndDrawing();

        if (WindowShouldClose()) break;
    }

    UIWidget_unref(widget);
    CloseWindow();
    return 0;
}

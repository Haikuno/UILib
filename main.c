#include "src/ui_widget.h"
#include <raylib.h>

int main(int argc, char *argv[]) {
    auto widget   = UI_Widget_create("color", 0x222222FF);
    auto widget_2 = GBL_NEW(UI_Widget, "x", 10.0f, "y", 10.0f, "color", 0xBA1C3CAA, "border_a", 255);
    UI_make_child(widget, widget_2);

    // init raylib
    InitWindow(640, 480, "UI Widget Example");
    SetTargetFPS(60);

    // draw it
    while (true) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        UI_draw(widget); // we only need to call draw on the parent

        // move widget around, bouncing with walls. widget 2 will follow
        for (static float dx = 2.0f, dy = 2.0f;;) {
            widget->x += dx;
            widget->y += dy;
            if (widget->x < 0 || widget->x + widget->w > 640) dx = -dx;
            if (widget->y < 0 || widget->y + widget->h > 480) dy = -dy;
            break;
        }

        EndDrawing();
        if (WindowShouldClose()) break;
    }

    UI_unref(widget); // only need to call unref on the parent
    CloseWindow();
    return 0;
}

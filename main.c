#include "src/ui_widget.h"
#include "src/ui_button.h"

int main(int argc, char *argv[]) {
    auto widget   = UI_Widget_create("color", 0x222222FF, "label", "I'm a widget!");
    auto widget_2 = GBL_NEW(UI_Widget, "x", 10.0f, "y", 10.0f, "color", 0xBA1C3CAA, "border_a", 255);
    auto widget_3 = GBL_OBJECT_NEW_N(UI_Widget, "x", 30.0f, "y", 50.0f, "w", 20.0f, "h", 200.0f, "border_a", 255, "r", 200);

    auto button = GBL_OBJECT_NEW_N(UI_Button, "x", 30.0f, "y", 120.0f, "w", 90.0f, "h", 90.0f, "label", "Button!");
    UI_make_child(widget, widget_2);
    UI_make_child(widget_2, button);

    // init raylib
    InitWindow(640, 480, "UI Widget Example");
    SetTargetFPS(60);

    // draw it
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        UI_draw(widget); // we only need to call draw on the parent
        UI_draw(widget_3);

        // move widget around, bouncing with walls. all children will follow
        for (static float dx = 2.0f, dy = 2.0f;;) {
            widget->x += dx;
            widget->y += dy;
            if (widget->x < 0 || widget->x + widget->w > 640) dx = -dx;
            if (widget->y < 0 || widget->y + widget->h > 480) dy = -dy;
            break;
        }

        EndDrawing();
    }

    UI_unref(widget); // only need to call unref on the parent
    UI_unref(widget_3);
    CloseWindow();
    return 0;
}

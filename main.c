#include "src/ui.h"

int main(int argc, char *argv[]) {
	auto container = UI_Container_create("color", 0x444444FF, "orientation", 'v');

	auto button    = UI_Button_create("label", "1", "color", 0x8888FFFF, "border_color", 0xFFFFFFFF);
	UI_make_child(container, button);

	// init raylib
	InitWindow(640, 480, "UI Widget Example");
	SetTargetFPS(60);

	// main loop
	while (!WindowShouldClose()) {

		BeginDrawing();
		ClearBackground(RAYWHITE);

		// shrink, enlarge, and move the container around
		if (IsKeyDown(KEY_RIGHT)) UI_WIDGET(container)->w += 2;
		if (IsKeyDown(KEY_LEFT))  UI_WIDGET(container)->w -= 2;
		if (IsKeyDown(KEY_UP))    UI_WIDGET(container)->h -= 2;
		if (IsKeyDown(KEY_DOWN))  UI_WIDGET(container)->h += 2;
		if (IsKeyDown(KEY_W))     UI_WIDGET(container)->y -= 2;
		if (IsKeyDown(KEY_S))     UI_WIDGET(container)->y += 2;
		if (IsKeyDown(KEY_A))     UI_WIDGET(container)->x -= 2;
		if (IsKeyDown(KEY_D))     UI_WIDGET(container)->x += 2;

		// if spacebar is pressed add new button to container
		if (IsKeyPressed(KEY_SPACE)) {
			size_t count = GblObject_childCount(GBL_OBJECT(container));
			char buf[16];
			snprintf(buf, sizeof(buf), "%d", count + 1);
			auto new_button = UI_Button_create("label", buf, "color", 0x8888FFFF, "border_color", 0xFFFFFFFF);
			UI_make_child(container, new_button);
		}

		// if backspace is pressed, remove the last button from the container
		if (IsKeyPressed(KEY_BACKSPACE)) {
			size_t count = GblObject_childCount(GBL_OBJECT(container));
			if (count > 0) {
				auto last = GblObject_findChildByIndex(GBL_OBJECT(container), count - 1);
				GblObject_removeChild(GBL_OBJECT(container), GBL_OBJECT(last));
				UI_unref(last); // unref the removed child
			}
		}

		// if F pressed, toggle orientation
		if (IsKeyPressed(KEY_F)) {
			UI_CONTAINER(container)->orientation = UI_CONTAINER(container)->orientation == 'h' ? 'v' : 'h';
		}

		UI_update(container);
		UI_draw(container);

		EndDrawing();
	}

	UI_unref(container); // only need to call unref on the parent
	CloseWindow();
	return 0;
}

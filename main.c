#include "src/ui.h"

int main(int argc, char *argv[]) {
	// init raylib
	InitWindow(640, 480, "UI Widget Example");
	SetTargetFPS(60);
	Font nimbus = LoadFont("../Nimbus.fnt");

	auto container = UI_Container_create("color", 0x444444FF, "orientation", 'v', "padding", 15.0f, "margin", 10.0f);
	auto button    = UI_Button_create("label", "Example text", "color", 0x8888FFFF, "border_color", 0xFFFFFFFF, "font", &nimbus);
	UI_add_child(container, button);

	// main loop
	while (!WindowShouldClose()) {

		BeginDrawing();
		ClearBackground(RAYWHITE);

		DrawTextEx(nimbus, "Press SPACE to add buttons to the container", 	(Vector2){ 10, 10 }, 20, 1, BLACK);
		DrawTextEx(nimbus, "Press BACKSPACE to remove them", 				(Vector2){ 10, 40 }, 20, 1, BLACK);

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
			auto new_button = UI_Button_create("label", "Example text", "color", 0x8888FFFF, "border_color", 0xFFFFFFFF, "font", &nimbus);
			UI_add_child(container, new_button);
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

#include "src/ui.h"

typedef struct {
	KeyboardKey key;
	UI_CONTROLLER_BUTTON button;
} KeyBinding;

void check_input(UI_Controller *controller) {
	static KeyBinding constexpr bindings[] = {
        { KEY_A,		UI_CONTROLLER_PRIMARY	},
        { KEY_S,		UI_CONTROLLER_SECONDARY	},
        { KEY_D,		UI_CONTROLLER_TERTIARY	},
		{ KEY_UP,		UI_CONTROLLER_UP		},
		{ KEY_RIGHT,	UI_CONTROLLER_RIGHT		},
		{ KEY_DOWN,		UI_CONTROLLER_DOWN		},
		{ KEY_LEFT,		UI_CONTROLLER_LEFT		}
    };
	static size_t constexpr n_bindings = sizeof(bindings) / sizeof(bindings[0]);

	for (size_t i = 0; i < n_bindings; ++i) {
		if (IsKeyPressed(bindings[i].key))
			UI_Controller_notify_button(controller, UI_CONTROLLER_BUTTON_PRESS, bindings[i].button);
		if (IsKeyReleased(bindings[i].key))
			UI_Controller_notify_button(controller, UI_CONTROLLER_BUTTON_RELEASE, bindings[i].button);
	}
}

void add_new_button_as_sibling (UI_Button *pSelf) {
	auto new_button = UI_Button_create("label", "dynamic button", "color", 0x8888FFFF, "border_color", 0xFFFFFFFF);
	UI_add_child(GblObject_parent(GBL_OBJECT(pSelf)), new_button);
}

void add_new_container_as_parents_sibling (UI_Button *pSelf) {
	GblObject *pParent = GblObject_parent(GBL_OBJECT(pSelf));

	const char orientation = rand() % 2 == 0 ? 'h' : 'v';

	auto new_container = UI_Container_create("color", 0x444444FF, "orientation", orientation, "padding", 15.0f, "margin", 10.0f);
	UI_add_child(GblObject_parent(pParent), new_container);

	auto button_1		= UI_Button_create("label", "new_button", "color", 0x8888FFFF, "border_color", 0xFFFFFFFF);
	GBL_CONNECT(button_1, "on_press", add_new_button_as_sibling);
	UI_add_child(new_container, button_1);
}

int main(int argc, char *argv[]) {
	// init raylib
	InitWindow(640, 480, "UI Widget Example");
	SetTargetFPS(60);
	Font nimbus = LoadFont("./Nimbus.fnt");

	auto root			= UI_Root_create();
	auto controller		= UI_Controller_create();
	auto main_container = UI_Container_create("color", 0x00000000, "orientation", 'h', "w", 640.0f, "h", 480.0f, "x", 0.0f, "y", 0.0f);
	auto container_1	= UI_Container_create("color", 0x444444FF, "orientation", 'h', "padding", 15.0f, "margin", 10.0f);
	auto button_1		= UI_Button_create("label", "new_button", "color", 0x8888FFFF, "border_color", 0xFFFFFFFF);
	auto button_2		= UI_Button_create("label", "new_container", "color", 0x8888FFFF, "border_color", 0xFFFFFFFF);

	Texture2D texture	= LoadTexture("./fish.png");
	auto widget			= UI_Widget_create("texture", &texture, "label", "cool fish!", "color", 0x8888FFFF);


	UI_add_child(root, main_container);
	UI_add_child(root, controller);
	UI_add_child(main_container, container_1);
	UI_add_child(container_1, button_1);
	UI_add_child(container_1, button_2);
	UI_add_child(container_1, widget);

	GBL_CONNECT(button_1, "on_press", add_new_button_as_sibling);
	GBL_CONNECT(button_2, "on_press", add_new_container_as_parents_sibling);

	// main loop
	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(RAYWHITE);

		check_input(controller);

		// only need to call update and draw on the top-most parent (usually root)
		UI_update(root);
		UI_draw(root);


		EndDrawing();
	}

	UI_unref(root); // only need to call unref on the top-most parent (usually root)
	CloseWindow();
	return 0;
}

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

int main(int argc, char *argv[]) {
	// init raylib
	InitWindow(640, 480, "UI Widget Example");
	SetTargetFPS(60);
	Font nimbus = LoadFont("./Nimbus.fnt");

	auto root		= UI_Root_create();
	auto controller = UI_Controller_create();
	auto container	= UI_Container_create("color", 0x444444FF, "orientation", 'v', "padding", 15.0f, "margin", 10.0f);
	auto button		= UI_Button_create("label", "Example text", "color", 0x8888FFFF, "border_color", 0xFFFFFFFF, "font", &nimbus);
	auto button_2	= UI_Button_create("label", "Example text 2", "color", 0x8888FFFF, "border_color", 0xFFFFFFFF, "font", &nimbus);

	UI_add_child(root, controller);
	UI_add_child(root, container);
	UI_add_child(container, button);
	UI_add_child(container, button_2);

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

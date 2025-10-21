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
	InitWindow(320, 240, "UILib Example");
	SetTargetFPS(60);

	auto root = UI_Root_create();
    auto controller = UI_Controller_create();
    UI_add_child(root, controller);

	auto container_parent = UI_Container_create("color", 0x00000000, "align_widgets", false, "resize_widgets", false); // <- This one used to not work, then it randomly started working.

    auto container_1 = UI_Container_create("x", 10.0f, "y", 10.0f, "w", 120.0f, "h", 150.0f, "orientation", 'v', "border_color", 0x000000FF,  "is_relative", false);
    auto container_2 = UI_Container_create("x", 140.0f, "y", 10.0f, "w", 170.0f, "h", 150.0f, "orientation", 'v', "border_color", 0x000000FF, "is_relative", false);
    auto container_3 = UI_Container_create("x", 10.0f, "y", 170.0f, "w", 300.0f, "h", 60.0f, "orientation", 'h', "border_color", 0x000000FF, "is_relative", false);

    UI_add_child(root, container_parent);
    UI_add_child(container_parent, container_1);
    UI_add_child(container_parent, container_2);
    UI_add_child(container_parent, container_3);

	auto button_1 = UI_Button_create("label", "1");
	auto button_2 = UI_Button_create("label", "2");
    auto button_3 = UI_Button_create("label", "3");
	auto button_4 = UI_Button_create("label", "4");
    auto button_5 = UI_Button_create("label", "5");


    auto button_select	= 	UI_Button_create("label", "Select");
    auto button_back 	= 	UI_Button_create("label", "Go Back");

	UI_add_child(container_1, button_1);
	UI_add_child(container_1, button_2);
    UI_add_child(container_1, button_3);
	UI_add_child(container_1, button_4);
    UI_add_child(container_1, button_5);

    UI_add_child(container_3, button_select);
	UI_add_child(container_3, button_back);

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

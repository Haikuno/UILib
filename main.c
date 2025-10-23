#include "src/ui.h"
#include <raylib.h>

typedef struct {
	KeyboardKey key;
	UI_CONTROLLER_BUTTON button_ui;
} KeyBinding;

typedef struct {
	GamepadButton button_gamepad;
	UI_CONTROLLER_BUTTON button_ui;
} ButtonBinding;

void check_input(UI_Controller *controller) {
	static KeyBinding constexpr key_bindings[] = {
		{ KEY_UP,		UI_CONTROLLER_UP		},
		{ KEY_RIGHT,	UI_CONTROLLER_RIGHT		},
		{ KEY_DOWN,		UI_CONTROLLER_DOWN		},
		{ KEY_LEFT,		UI_CONTROLLER_LEFT		},
        { KEY_A,		UI_CONTROLLER_PRIMARY	},
		{ KEY_S,		UI_CONTROLLER_SECONDARY	},
		{ KEY_D,		UI_CONTROLLER_TERTIARY	},
    };
	static size_t constexpr n_key_bindings = sizeof(key_bindings) / sizeof(key_bindings[0]);

	static ButtonBinding constexpr button_bindings[] = {
		{ 	GAMEPAD_BUTTON_LEFT_FACE_UP, 		UI_CONTROLLER_UP 		},
		{	GAMEPAD_BUTTON_LEFT_FACE_RIGHT, 	UI_CONTROLLER_RIGHT 	},
		{	GAMEPAD_BUTTON_LEFT_FACE_DOWN, 		UI_CONTROLLER_DOWN 		},
		{	GAMEPAD_BUTTON_LEFT_FACE_LEFT, 		UI_CONTROLLER_LEFT		},
		{	GAMEPAD_BUTTON_RIGHT_FACE_DOWN, 	UI_CONTROLLER_PRIMARY	},
		{	GAMEPAD_BUTTON_RIGHT_FACE_RIGHT,	UI_CONTROLLER_SECONDARY	},
		{	GAMEPAD_BUTTON_RIGHT_FACE_UP, 		UI_CONTROLLER_TERTIARY	},
	};
	static size_t constexpr n_button_bindings = sizeof(button_bindings) / sizeof(button_bindings[0]);

	if (controller->isKeyboard) {
		for (size_t i = 0; i < n_key_bindings; ++i) {
			if (IsKeyPressed(key_bindings[i].key))
				UI_Controller_notifyButton(controller, UI_CONTROLLER_BUTTON_PRESS, key_bindings[i].button_ui);
			if (IsKeyReleased(key_bindings[i].key))
				UI_Controller_notifyButton(controller, UI_CONTROLLER_BUTTON_RELEASE, key_bindings[i].button_ui);
		}

		return;
	}

	for (size_t i = 0; i < n_button_bindings; ++i) {
		if (IsGamepadButtonPressed(controller->controllerId, button_bindings[i].button_gamepad))
			UI_Controller_notifyButton(controller, UI_CONTROLLER_BUTTON_PRESS, button_bindings[i].button_ui);
		if (IsGamepadButtonReleased(controller->controllerId, button_bindings[i].button_gamepad))
			UI_Controller_notifyButton(controller, UI_CONTROLLER_BUTTON_RELEASE, button_bindings[i].button_ui);
	}
}

int main(int argc, char *argv[]) {
	InitWindow(320, 240, "UILib Example");
	SetTargetFPS(60);

	auto root = UI_Root_create();
    auto controller 	= UI_Controller_create("color", 0xFF0000FF);
    auto controller_2 	= UI_Controller_create("color", 0x0000FFFF, "isKeyboard", true, "controllerId", 1);
    UI_add_child(root, controller);
    UI_add_child(root, controller_2);

	auto container_parent = UI_Container_create("color", 0x00000000, "alignWidgets", false, "resizeWidgets", false); // <- This one used to not work, then it randomly started working.

    auto container_1 = UI_Container_create("x", 10.0f, "y", 10.0f, "w", 120.0f, "h", 150.0f, "orientation", 'v', "border_color", 0x000000FF,  "isRelative", false);
    auto container_2 = UI_Container_create("x", 140.0f, "y", 10.0f, "w", 170.0f, "h", 150.0f, "orientation", 'v', "border_color", 0x000000FF, "isRelative", false);
    auto container_3 = UI_Container_create("x", 10.0f, "y", 170.0f, "w", 300.0f, "h", 60.0f, "orientation", 'h', "border_color", 0x000000FF, "isRelative", false);

    UI_add_child(root, container_parent);
    UI_add_child(container_parent, container_1);
    UI_add_child(container_parent, container_2);
    UI_add_child(container_parent, container_3);

	auto button_1 = UI_Button_create("label", "1", "border_color", 0x000000FF);
	auto button_2 = UI_Button_create("label", "2", "isSelectedByDefault", true);
    auto button_3 = UI_Button_create("label", "3");
	auto button_4 = UI_Button_create("label", "4");
    auto button_5 = UI_Button_create("label", "5");


    auto button_select	= 	UI_Button_create("label", "Select");
    auto button_back 	= 	UI_Button_create("label", "Go Back");


	auto fishPicTexture = LoadTexture("babyfish_test.png");

	auto fishPic		= UI_Widget_create("a", 0, "texture", &fishPicTexture, "label", "myverycoolfish");

	UI_add_child(container_1, button_1);
	UI_add_child(container_1, button_2);
    UI_add_child(container_1, button_3);
	UI_add_child(container_1, button_4);
    UI_add_child(container_1, button_5);

	UI_add_child(container_2, fishPic);

    UI_add_child(container_3, button_select);
	UI_add_child(container_3, button_back);

	// main loop
	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(RAYWHITE);

		check_input(controller);
		check_input(controller_2);

		// only need to call update and draw on the top-most parent (usually root)
		UI_update(root);
		UI_draw();


		EndDrawing();
	}

	UI_unref(root); // only need to call unref on the top-most parent (usually root)
	CloseWindow();
	return 0;
}

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

void checkInput(UI_Controller *controller) {
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
	auto fishImgTexture = LoadTexture("babyfish_test.png");

	auto root 			= UI_Root_create();
    auto controller 	= UI_Controller_create("color", 0xFF0F0FFF, "parent", root);
    auto controller_2 	= UI_Controller_create("controllerId", 1, "color", 0x0000FFFF, "isKeyboard", true, "parent", root);

	auto container_parent = UI_Container_create("a", 0, "alignWidgets", false, "resizeWidgets", false, "parent", root);

    auto container_1 = UI_Container_create(
											"x", 10.0f, "y", 10.0f, "w", 120.0f, "h", 150.0f,
											"color", 0x000000FF,
											"orientation", 'v',
											"border_color", 0x000000FF,
											"parent", container_parent);

    auto container_2 = UI_Container_create(	"x", 140.0f, "y", 10.0f, "w", 170.0f, "h", 150.0f,
											"color", 0xE08616FF, "border_color", 0x000000FF,
											"orientation", 'v',
											"parent", container_parent);

	auto container_3 = UI_Container_create(	"x", 10.0f, "y", 170.0f, "w", 300.0f, "h", 60.0f,
											"color", 0x000000FF, "border_color", 0x000000FF,
											"orientation", 'h', "margin", 8.0f,
											"parent", container_parent);


	auto button_1 = UI_Button_create(	"label", "1",
										"color", 0xE08616FF,
										"border_radius", 0.5f, "border_color", 0x000000FF,
										"parent", container_1);

	auto button_2 = UI_Button_create(	"label", "2",
										"color", 0xE08616FF,
										"border_radius", 0.5f, "border_color", 0x000000FF,
										"parent", container_1);

    auto button_3 = UI_Button_create(	"label", "3",
										"color", 0xE08616FF,
										"border_radius", 0.5f, "border_color", 0x000000FF,
										"parent", container_1);

	auto button_4 = UI_Button_create(	"label", "4",
										"color", 0xE08616FF,
										"border_radius", 0.5f, "border_color", 0x000000FF,
										"parent", container_1);

    auto button_5 = UI_Button_create(	"label", "5",
										"color", 0xE08616FF,
										"border_radius", 0.5f, "border_color", 0x000000FF,
										"parent", container_1);

	auto fishImg		= UI_Widget_create(	"a", 0, "texture", &fishImgTexture,
											"label", "cool fish!",
											"parent", container_2);

	auto button_select	= 	UI_Button_create(	"label", "Select",
												"color", 0xE08616FF, "border_color", 0x000000FF,
												"parent", container_3);
    auto button_back 	= 	UI_Button_create(	"label", "Go Back",
												"color", 0xE08616FF, "border_color", 0x000000FF,
												"parent", container_3);


	// main loop
	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(RAYWHITE);

		checkInput(controller);
		checkInput(controller_2);

		// only need to call update on the top-most parent (usually root)
		UI_update(root);
		UI_draw();

		EndDrawing();
	}

	UnloadTexture(fishImgTexture);
	UI_unref(root); // only need to call unref on the top-most parent (usually root)
	CloseWindow();
	return 0;
}

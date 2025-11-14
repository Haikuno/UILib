#include <gumball/gumball.h>
#include <raylib.h>

int main(int argc, char *argv[]) {
	InitWindow(320, 240, "UILib Example");
	SetTargetFPS(60);

	auto root 		= GUM_Root_create();

    auto controller = GUM_Controller_create("controllerId", 1, "color", 0xFF0000FF, "isKeyboard", true, "name", "main controller");

	auto container_parent = GUM_Container_create("a", 0, "alignWidgets", false, "resizeWidgets", false, "isRelative", false, "name", "parent container");

    auto container_1 = GUM_Container_create("x", 10.0f, "y", 10.0f, "w", 120.0f, "h", 150.0f,
										   "color", 0x000000FF,
										   "orientation", 'v',
										   "border_color", 0x000000FF,
										   "parent", container_parent,
											"name", "container 1");

    auto container_2 = GUM_Container_create("x", 140.0f, "y", 10.0f, "w", 170.0f, "h", 150.0f,
										   "color", 0xE08616FF, "border_color", 0x000000FF,
										   "orientation", 'v',
										   "parent", container_parent, "name", "container 2");

	auto container_3 = GUM_Container_create("x", 10.0f, "y", 170.0f, "w", 300.0f, "h", 60.0f,
										   "color", 0x000000FF, "border_color", 0x000000FF,
										   "orientation", 'h', "margin", 8.0f,
										   "parent", container_parent, "name", "container 3");

	auto button_1 = GUM_Button_create("label", "1",
									 "color", 0xE08616FF,
									 "border_radius", 0.5f, "border_color", 0x000000FF,
									 "parent", container_1, "name", "button 1");

	auto button_2 = GUM_Button_create("label", "X",
									 "color", 0xE08616FF,
									 "border_radius", 0.5f, "border_color", 0x000000FF,
									 "parent", container_1, "isSelectable", false, "name", "button 2");

    auto button_3 = GUM_Button_create("label", "3",
									 "color", 0xE08616FF,
									 "border_radius", 0.5f, "border_color", 0x000000FF,
									 "parent", container_1, "name", "button 3");

	auto button_4 = GUM_Button_create("label", "4",
									 "color", 0xE08616FF,
									 "border_radius", 0.5f, "border_color", 0x000000FF,
									 "parent", container_1, "name", "button 4");

    auto button_5 = GUM_Button_create("label", "X",
									 "color", 0xE08616FF,
									 "border_radius", 0.5f, "border_color", 0x000000FF,
									 "parent", container_1, "isSelectable", false, "name", "button 5");

	GUM_IResource *pRes = GUM_Manager_load("../babyfish_test.png");

	auto fishImg  = GUM_Widget_create("a", 0, "label", "cool fish!",
									  "parent", container_2, "name", "fishimg",
									  "texture", pRes);

	auto button_select	= GUM_Button_create("label", "Select",
										   "color", 0xE08616FF, "border_color", 0x000000FF,
										   "isSelectedByDefault", true,
										   "parent", container_3, "name", "select");

    auto button_back 	= GUM_Button_create("label", "Go Back",
										    "color", 0xE08616FF, "border_color", 0x000000FF,
										    "parent", container_3, "name", "back");

	// main loop
	while (!WindowShouldClose()) {
		GUM_update(root);

		BeginDrawing();
		ClearBackground(RAYWHITE);
		GUM_draw(nullptr);
		EndDrawing();
	}

	GUM_unref(root);
	CloseWindow();
	return 0;
}

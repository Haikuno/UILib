#ifndef UI_BUTTONEVENT_H
#define UI_BUTTONEVENT_H

#include "ui_event.h"

typedef enum {
    UI_CONTROLLER_BUTTON_PRESS,
    UI_CONTROLLER_BUTTON_RELEASE
} UI_CONTROLLER_BUTTON_STATE;

typedef enum {
	UI_CONTROLLER_UP,
	UI_CONTROLLER_RIGHT,
	UI_CONTROLLER_DOWN,
	UI_CONTROLLER_LEFT,
	UI_CONTROLLER_PRIMARY,
	UI_CONTROLLER_SECONDARY,
	UI_CONTROLLER_TERTIARY
} UI_CONTROLLER_BUTTON;

typedef struct {
    UI_Event					base;	// "inherit" from your UI_Event so it's a libGimbal type
    UI_CONTROLLER_BUTTON_STATE	state;	// what kind of event is this?
    UI_CONTROLLER_BUTTON 		button;	// which button pressed?
} UI_ButtonEvent;

UI_ButtonEvent *UI_ButtonEvent_create(void *pController, UI_CONTROLLER_BUTTON_STATE state, UI_CONTROLLER_BUTTON button);

#endif
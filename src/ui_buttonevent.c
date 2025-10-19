#include "ui_buttonevent.h"

UI_ButtonEvent* UI_ButtonEvent_create(UI_CONTROLLER_BUTTON_STATE state, UI_CONTROLLER_BUTTON button) {
	UI_ButtonEvent* pEvent =
		(UI_ButtonEvent *)GblBox_create(UI_EVENT_TYPE,
										sizeof(UI_ButtonEvent),
										nullptr,
										nullptr,
										nullptr);
	pEvent->state = state;
	pEvent->button = button;
	return pEvent;
}
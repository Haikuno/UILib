#ifndef UI_CONTROLLER_H
#define UI_CONTROLLER_H

#include "ui_button.h"
#include "ui_buttonevent.h"

#define UI_CONTROLLER_TYPE					(GBL_TYPEID(UI_Controller))
#define UI_CONTROLLER(self)					(GBL_CAST(UI_Controller, self))
#define UI_CONTROLLER_CLASS(klass)			(GBL_CLASS_CAST(UI_Controller, klass))
#define UI_CONTROLLER_GET_CLASS(self)		(GBL_CLASSOF(UI_Controller, self))

#define GBL_SELF_TYPE UI_Controller

GBL_FORWARD_DECLARE_STRUCT(UI_Controller);
GBL_DECLS_BEGIN

GBL_CLASS_DERIVE_EMPTY(UI_Controller, UI_Widget)

GBL_INSTANCE_DERIVE(UI_Controller, UI_Widget)
	UI_Button* 	pSelectedButton;
	uint8_t		controllerId;	 // To know which controller is currently selected
	bool		isKeyboard;
GBL_INSTANCE_END

GBL_PROPERTIES(UI_Controller,
	(controllerId,		GBL_GENERIC, (READ, WRITE), GBL_UINT8_TYPE),
	(isKeyboard,		GBL_GENERIC, (READ, WRITE), GBL_BOOL_TYPE)
)


GblType UI_Controller_type(void);

#define UI_Controller_create(...) GBL_NEW(UI_Controller __VA_OPT__(,) __VA_ARGS__)

/*
	Takes in a UI_Controller instance,
	a button state (UI_CONTROLLER_BUTTON_PRESS or UI_CONTROLLER_BUTTON_RELEASE),
	and a button (UI_CONTROLLER_UP, UI_CONTROLLER_RIGHT, etc.)

	Sends it to the parent (usually a UI_Root) for it to handle
*/
void UI_Controller_notifyButton(GBL_SELF, UI_CONTROLLER_BUTTON_STATE state, UI_CONTROLLER_BUTTON button);

GBL_DECLS_END
#undef GBL_SELF_TYPE

#endif // UI_CONTROLLER_H

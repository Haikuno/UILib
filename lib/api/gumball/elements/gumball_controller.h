#ifndef GUM_CONTROLLER_H
#define GUM_CONTROLLER_H

#include "gumball_button.h"

#define GUM_CONTROLLER_TYPE					(GBL_TYPEID(GUM_Controller))
#define GUM_CONTROLLER(self)					(GBL_CAST(GUM_Controller, self))
#define GUM_CONTROLLER_CLASS(klass)			(GBL_CLASS_CAST(GUM_Controller, klass))
#define GUM_CONTROLLER_CLASSOF(self)			(GBL_CLASSOF(GUM_Controller, self))

#define GBL_SELF_TYPE GUM_Controller

GBL_FORWARD_DECLARE_STRUCT(GUM_Controller);
GBL_DECLS_BEGIN

typedef enum {
    GUM_CONTROLLER_BUTTON_PRESS,
    GUM_CONTROLLER_BUTTON_RELEASE
} GUM_CONTROLLER_BUTTON_STATE;

typedef enum {
	GUM_CONTROLLER_UP,
	GUM_CONTROLLER_RIGHT,
	GUM_CONTROLLER_DOWN,
	GUM_CONTROLLER_LEFT,
	GUM_CONTROLLER_PRIMARY,
	GUM_CONTROLLER_SECONDARY,
	GUM_CONTROLLER_TERTIARY
} GUM_CONTROLLER_BUTTON;

GBL_CLASS_DERIVE_EMPTY(GUM_Controller, GUM_Widget)

GBL_INSTANCE_DERIVE(GUM_Controller, GUM_Widget)
	GUM_Button* 	pSelectedButton; // The currently selected button
	uint8_t		controllerId;	 // To know which controller is currently selected
	bool		isKeyboard; 	 // If the controller is a keyboard
GBL_INSTANCE_END

GBL_PROPERTIES(GUM_Controller,
	(controllerId,		GBL_GENERIC, (READ, WRITE), GBL_UINT8_TYPE),
	(isKeyboard,		GBL_GENERIC, (READ, WRITE), GBL_BOOL_TYPE)
)


GblType GUM_Controller_type(void);

// Takes a list of Name/Value pairs
#define GUM_Controller_create(...) 		GBL_NEW(GUM_Controller __VA_OPT__(,) __VA_ARGS__)

/*
	Takes in a GUM_Controller,
	a button state (GUM_CONTROLLER_BUTTON_PRESS or GUM_CONTROLLER_BUTTON_RELEASE),
	and a button (GUM_CONTROLLER_UP, GUM_CONTROLLER_RIGHT, etc.), and handles the event
*/
void GUM_Controller_sendButton			(GBL_SELF, GUM_CONTROLLER_BUTTON_STATE state, GUM_CONTROLLER_BUTTON button);

// Takes in a GUM_Controller, and sets its selected button to the passed GUM_Button
void GUM_Controller_setSelectedButton	(GBL_SELF, GUM_Button* pButton);

GBL_DECLS_END
#undef GBL_SELF_TYPE

#endif // GUM_CONTROLLER_H

#ifndef UI_BUTTON_H
#define UI_BUTTON_H

#include "ui_widget.h"

#define UI_BUTTON_TYPE				(GBL_TYPEID(UI_Button))
#define UI_BUTTON(self)				(GBL_CAST(UI_Button, self))
#define UI_BUTTON_CLASS(klass)		(GBL_CLASS_CAST(UI_Button, klass))
#define UI_BUTTON_CLASSOF(self)		(GBL_CLASSOF(UI_Button, self))

#define GBL_SELF_TYPE UI_Button

GBL_FORWARD_DECLARE_STRUCT(UI_Button);
GBL_DECLS_BEGIN

GBL_CLASS_DERIVE_EMPTY(UI_Button, UI_Widget)

// TODO: flags
GBL_INSTANCE_DERIVE(UI_Button, UI_Widget)
	bool 			isActive;				// if this button can be pressed
	bool 			isSelectable;			// if this button can be selected
	bool 			isSelected;				// if this button is currently selected
	bool 			isSelectedByDefault;	// if this button is selected by default
	GblArrayList
GBL_INSTANCE_END

GBL_PROPERTIES(UI_Button,
	(isActive,				GBL_GENERIC, (READ, WRITE), GBL_BOOL_TYPE),
	(isSelectable,			GBL_GENERIC, (READ, WRITE), GBL_BOOL_TYPE),
	(isSelectedByDefault,	GBL_GENERIC, (READ, WRITE), GBL_BOOL_TYPE)
)


// You can connect to the signals with UI_connect() like so:
// UI_connect(button, onPressPrimary, myFunction);
GBL_SIGNALS(UI_Button,
	(onPressPrimary, 	(GBL_INSTANCE_TYPE, pReceiver)),
	(onPressSecondary, 	(GBL_INSTANCE_TYPE, pReceiver)),
	(onPressTertiary, 	(GBL_INSTANCE_TYPE, pReceiver))
)

GblType UI_Button_type(void);

// Takes a list of Name/Value pairs
#define UI_Button_create(/* propertyName, propertyValue */ ...) GBL_NEW(UI_Button __VA_OPT__(,) __VA_ARGS__)

GBL_DECLS_END
#undef GBL_SELF_TYPE

#endif // UI_BUTTON_H

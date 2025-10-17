#ifndef UI_BUTTON_H
#define UI_BUTTON_H

#include "ui_widget.h"

#define UI_BUTTON_TYPE				(GBL_TYPEID(UI_Button))
#define UI_BUTTON(self)				(GBL_CAST(UI_Button, self))
#define UI_BUTTON_CLASS(klass)		(GBL_CLASS_CAST(UI_Button, klass))
#define UI_BUTTON_GET_CLASS(self)	(GBL_CLASSOF(UI_Button, self))

#define GBL_SELF_TYPE UI_Button

GBL_FORWARD_DECLARE_STRUCT(UI_Button);
GBL_DECLS_BEGIN

GBL_CLASS_DERIVE(UI_Button, UI_Widget)
	//
GBL_CLASS_END

GBL_INSTANCE_DERIVE(UI_Button, UI_Widget)
	bool is_active;
GBL_INSTANCE_END

GBL_PROPERTIES(UI_Button,
	(is_active,	GBL_GENERIC, (READ, WRITE), GBL_BOOL_TYPE)
)

GblType UI_Button_type(void);

UI_Button* UI_Button_ref(UI_Button* pSelf);

GBL_DECLS_END

#undef GBL_SELF_TYPE

#define UI_Button_create(...) GBL_NEW(UI_Button __VA_OPT__(,) __VA_ARGS__)

#endif // UI_BUTTON_H

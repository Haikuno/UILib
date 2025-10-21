#ifndef UI_LIB_H
#define UI_LIB_H

#include "ui_root.h"
#include "ui_controller.h"
#include "ui_widget.h"
#include "ui_button.h"
#include "ui_container.h"
#include "ui_font.h"

// Takes in an UI element, updates it and all of its children.
#define UI_update(obj)					(UI_update_		(GBL_OBJECT(obj)))

// Takes in an UI element, draws it and all of its children.
#define UI_draw(obj)					(UI_draw_		(GBL_OBJECT(obj)))

// Takes in two UI elements, adds the second one as a child of the first.
#define UI_add_child(self, child)		(GblObject_addChild(GBL_OBJECT(self), GBL_OBJECT(child)))

// Decrements the reference count of a UI element, freeing it if it reaches zero.
// Also recursively unrefs all of its children.
#define UI_unref(obj)					(UI_unref_		(GBL_OBJECT(obj)))

// Connects an UI element's (typically a UI_Button) signal to a function.
// See UI_button.h
#define UI_connect(obj, signal, func) 	(GBL_CONNECT(obj, signal, func))

#endif // UI_LIB_H

#ifndef UI_LIB_H
#define UI_LIB_H

#include "ui_widget.h"
#include "ui_button.h"
#include "ui_container.h"
#include "ui_font.h"

// Takes in an UI element, updates it and all of its children.
#define UI_update(obj)					(UI_update_		(UI_WIDGET(obj)))

// Takes in an UI element, draws it and all of its children.
#define UI_draw(obj)					(UI_draw_		(UI_WIDGET(obj)))

// Takes in two UI elements, adds the second one as a child of the first.
#define UI_add_child(parent, child)		(UI_add_child_	(UI_WIDGET(parent), UI_WIDGET(child)))

// Decrements the reference count of a UI element, freeing it if it reaches zero.
#define UI_unref(obj)					(UI_unref_		(UI_WIDGET(obj)))

#endif // UI_LIB_H

#ifndef UI_LIB_H
#define UI_LIB_H

#include "ui_widget.h"
#include "ui_button.h"
#include "ui_container.h"
#include "ui_font.h"

#define UI_update(obj)					(UI_update_(UI_WIDGET(obj)))
#define UI_draw(obj)					(UI_draw_(UI_WIDGET(obj)))
#define UI_make_child(parent, child)	(UI_make_child_(UI_WIDGET(parent), UI_WIDGET(child)))
#define UI_unref(obj)					(UI_unref_(UI_WIDGET(obj)))

#endif // UI_LIB_H

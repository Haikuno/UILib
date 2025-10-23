#ifndef UI_GENERAL_H
#define UI_GENERAL_H

#include <gimbal/gimbal_meta.h>

// Takes in a UI element, updates it and all of its children.
#define UI_update(obj)					(UI_update_		(GBL_OBJECT(obj)))

// Takes in two UI elements, adds the second one as a child of the first.
#define UI_add_child(self, child)		(GblObject_addChild(GBL_OBJECT(self), GBL_OBJECT(child)))

// Takes in two UI elements, removes the second one as a child of the first.
#define UI_remove_child(self, child)	(GblObject_removeChild(GBL_OBJECT(self), GBL_OBJECT(child)))

// Takes in a UI element and an index, returns the child of that element at that index as a
// GblObject that you can cast to the appropriate type.
// Example : UI_Widget *pChild = UI_WIDGET(UI_get_child_at(pObj, 0));
#define UI_get_child_at(self, index)	(GblObject_findChildByIndex(GBL_OBJECT(self), index))

// Decrements the reference count of a UI element, freeing it if it reaches zero.
// Also recursively unrefs all of its children.
#define UI_unref(obj)					(UI_unref_		(GBL_OBJECT(obj)))

// Connects a UI element's (typically a UI_Button) signal to a function.
// See UI_button.h
#define UI_connect(obj, signal, func) 	(GBL_CONNECT(obj, signal, func))

// Draws all of the UI elements in the draw queue
// Drawable elements are added to the draw queue when they are created
// Elements are drawn from z-index 0 to z-index 255, with 255 being the front
// If two elements have the same z-index, they are drawn in the order they were created (or enabled!)
GBL_RESULT	UI_draw(void);


// Disables drawing for a given widget
#define UI_draw_disable(obj) (UI_draw_disable_(GBL_OBJECT(obj)));

// Enables drawing for a given widget
#define UI_draw_enable(obj) (UI_draw_enable_(GBL_OBJECT(obj)));

// Disables drawing for a given widget and all of its children
#define UI_draw_disableAll(obj) (UI_draw_disableAll_(GBL_OBJECT(obj)));

// Enables drawing for a given widget and all of its children
#define UI_draw_enableAll(obj) (UI_draw_enableAll_(GBL_OBJECT(obj)));

// Functions used for macros. Ignore.
GBL_RESULT	UI_update_(GblObject *pSelf);
GBL_RESULT	UI_unref_(GblObject *pSelf);
void		UI_draw_enable_(GblObject *pSelf);
void		UI_draw_disable_(GblObject *pSelf);
void		UI_draw_enableAll_(GblObject *pSelf);
void		UI_draw_disableAll_(GblObject *pSelf);

#endif
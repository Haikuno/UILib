#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <gimbal/gimbal_meta.h>

#define UI_WIDGET_TYPE (GBL_TYPEID(UI_Widget))
#define UI_WIDGET(self) (GBL_CAST(UI_Widget, self))
#define UI_WIDGET_CLASS(klass) (GBL_CLASS_CAST(UI_Widget, klass))
#define UI_WIDGET_GET_CLASS(self) (GBL_CLASSOF(UI_Widget, self))

#define GBL_SELF_TYPE UI_Widget

GBL_FORWARD_DECLARE_STRUCT(UI_Widget);
GBL_DECLS_BEGIN

GBL_CLASS_DERIVE(UI_Widget, GblObject)
    GBL_RESULT (*pFnActivate)(GBL_SELF);
    GBL_RESULT (*pFnDeactivate)(GBL_SELF);
    void (*pFnDraw)(GBL_SELF);
GBL_CLASS_END

GBL_INSTANCE_DERIVE(UI_Widget, GblObject)
    float   x, y, w, h; // if the widget is a child of another widget, then x & y are relative
    uint8_t r, g, b, a;
    uint8_t border_r, border_g, border_b, border_a, border_width;
    bool    is_relative; // if the position of this widget should be relative to its parent (if it has one)
GBL_INSTANCE_END

GBL_PROPERTIES(UI_Widget,
    (x,             GBL_GENERIC, (READ, WRITE, LOAD, SAVE), GBL_FLOAT_TYPE),
    (y,             GBL_GENERIC, (READ, WRITE, LOAD, SAVE), GBL_FLOAT_TYPE),
    (w,             GBL_GENERIC, (READ, WRITE, LOAD, SAVE), GBL_FLOAT_TYPE),
    (h,             GBL_GENERIC, (READ, WRITE, LOAD, SAVE), GBL_FLOAT_TYPE),
    (is_relative,      GBL_GENERIC, (READ, WRITE, LOAD, SAVE), GBL_BOOL_TYPE),
    (color,         GBL_GENERIC, (READ, WRITE, LOAD, SAVE), GBL_UINT32_TYPE),
    (border_color,  GBL_GENERIC, (READ, WRITE, LOAD, SAVE), GBL_UINT32_TYPE),
    (r,             GBL_GENERIC, (READ, WRITE, LOAD, SAVE), GBL_UINT8_TYPE),
    (g,             GBL_GENERIC, (READ, WRITE, LOAD, SAVE), GBL_UINT8_TYPE),
    (b,             GBL_GENERIC, (READ, WRITE, LOAD, SAVE), GBL_UINT8_TYPE),
    (a,             GBL_GENERIC, (READ, WRITE, LOAD, SAVE), GBL_UINT8_TYPE),
    (border_r,      GBL_GENERIC, (READ, WRITE, LOAD, SAVE), GBL_UINT8_TYPE),
    (border_g,      GBL_GENERIC, (READ, WRITE, LOAD, SAVE), GBL_UINT8_TYPE),
    (border_b,      GBL_GENERIC, (READ, WRITE, LOAD, SAVE), GBL_UINT8_TYPE),
    (border_a,      GBL_GENERIC, (READ, WRITE, LOAD, SAVE), GBL_UINT8_TYPE),
    (border_width,  GBL_GENERIC, (READ, WRITE, LOAD, SAVE), GBL_UINT8_TYPE)
)

GblType UI_Widget_type(void);

UI_Widget* UI_Widget_ref(UI_Widget* pSelf);

void UI_unref(UI_Widget* pSelf);

void UI_make_child(UI_Widget* pParent, UI_Widget* pChild);

GBL_RESULT UI_draw(UI_Widget* pSelf);

GBL_DECLS_END

#undef GBL_SELF_TYPE

#define UI_Widget_create(...) GBL_NEW(UI_Widget __VA_OPT__(,) __VA_ARGS__)

#endif // UI_WIDGET_H

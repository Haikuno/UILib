#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <gimbal/gimbal_meta.h>

#define UI_WIDGET_TYPE (GBL_TYPEID(UIWidget))
#define UI_WIDGET(self) (GBL_CAST(UIWidget, self))
#define UI_WIDGET_CLASS(klass) (GBL_CLASS_CAST(UIWidget, klass))
#define UI_WIDGET_GET_CLASS(self) (GBL_CLASSOF(UIWidget, self))

#define GBL_SELF_TYPE UIWidget

GBL_FORWARD_DECLARE_STRUCT(UIWidget);
GBL_DECLS_BEGIN

GBL_CLASS_DERIVE(UIWidget, GblObject)
    GBL_RESULT (*pFnActivate)(GBL_SELF);
    GBL_RESULT (*pFnDeactivate)(GBL_SELF);
    void (*pFnDraw)(GBL_SELF);
GBL_CLASS_END

GBL_INSTANCE_DERIVE(UIWidget, GblObject)
    float x, y, w, h;
    uint8_t r, g, b, a;
GBL_INSTANCE_END

GBL_PROPERTIES(UIWidget,
    (x,     GBL_GENERIC, (READ, WRITE, LOAD, SAVE), GBL_FLOAT_TYPE),
    (y,     GBL_GENERIC, (READ, WRITE, LOAD, SAVE), GBL_FLOAT_TYPE),
    (w,     GBL_GENERIC, (READ, WRITE, LOAD, SAVE), GBL_FLOAT_TYPE),
    (h,     GBL_GENERIC, (READ, WRITE, LOAD, SAVE), GBL_FLOAT_TYPE),
    (color, GBL_GENERIC, (READ, WRITE, LOAD, SAVE), GBL_UINT32_TYPE),
    (r,     GBL_GENERIC, (READ, WRITE, LOAD, SAVE), GBL_UINT8_TYPE),
    (g,     GBL_GENERIC, (READ, WRITE, LOAD, SAVE), GBL_UINT8_TYPE),
    (b,     GBL_GENERIC, (READ, WRITE, LOAD, SAVE), GBL_UINT8_TYPE),
    (a,     GBL_GENERIC, (READ, WRITE, LOAD, SAVE), GBL_UINT8_TYPE)
)

GblType UIWidget_type(void);

UIWidget* UIWidget_ref(UIWidget* widget);

void UIWidget_unref(UIWidget* widget);

GBL_RESULT UIWidget_draw(UIWidget* pSelf);

GBL_DECLS_END


#undef GBL_SELF_TYPE

#define UIWidget_create(...) GBL_NEW(UIWidget __VA_OPT__(,) __VA_ARGS__)

#endif // UI_WIDGET_H

#ifndef UI_CONTAINER_H
#define UI_CONTAINER_H

#include "ui_widget.h"

#define UI_CONTAINER_TYPE				(GBL_TYPEID(UI_Container))
#define UI_CONTAINER(self)				(GBL_CAST(UI_Container, self))
#define UI_CONTAINER_CLASS(klass)		(GBL_CLASS_CAST(UI_Container, klass))
#define UI_CONTAINER_GET_CLASS(self)	(GBL_CLASSOF(UI_Container, self))

#define GBL_SELF_TYPE UI_Container

GBL_FORWARD_DECLARE_STRUCT(UI_Container);
GBL_DECLS_BEGIN

GBL_CLASS_DERIVE_EMPTY(UI_Container, UI_Widget)

GBL_INSTANCE_DERIVE(UI_Container, UI_Widget)
    char orientation;       // 'h' for horizontal, 'v' for vertical layout of child widgets
    bool resize_widgets;    // if true, child widgets will be resized to fit the container's width/height
    bool align_widgets;     // if true, child widgets will be aligned automatically
    float padding;          // the space between the container's border and its child widgets
    float margin;           // the space between child widgets
GBL_INSTANCE_END

GBL_PROPERTIES(UI_Container,
    (orientation,       GBL_GENERIC, (READ, WRITE), GBL_CHAR_TYPE),
    (resize_widgets,    GBL_GENERIC, (READ, WRITE), GBL_BOOL_TYPE),
    (align_widgets,     GBL_GENERIC, (READ, WRITE), GBL_BOOL_TYPE),
    (padding,           GBL_GENERIC, (READ, WRITE), GBL_FLOAT_TYPE),
    (margin,            GBL_GENERIC, (READ, WRITE), GBL_FLOAT_TYPE)
)

GblType UI_Container_type(void);

// Takes a list of Name/Value pairs
#define UI_Container_create(/* property_name, property_value */ ...) GBL_NEW(UI_Container __VA_OPT__(,) __VA_ARGS__)

GBL_DECLS_END
#undef GBL_SELF_TYPE

#endif // UI_CONTAINER_H

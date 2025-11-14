#ifndef GUM_CONTAINER_H
#define GUM_CONTAINER_H

#include "gumball_widget.h"

#define GUM_CONTAINER_TYPE				(GBL_TYPEID(GUM_Container))
#define GUM_CONTAINER(self)				(GBL_CAST(GUM_Container, self))
#define GUM_CONTAINER_CLASS(klass)		(GBL_CLASS_CAST(GUM_Container, klass))
#define GUM_CONTAINER_CLASSOF(self)	(GBL_CLASSOF(GUM_Container, self))

#define GBL_SELF_TYPE GUM_Container

GBL_FORWARD_DECLARE_STRUCT(GUM_Container);
GBL_DECLS_BEGIN

GBL_CLASS_DERIVE_EMPTY(GUM_Container, GUM_Widget)

GBL_INSTANCE_DERIVE(GUM_Container, GUM_Widget)
    char orientation;       // 'h' for horizontal, 'v' for vertical layout of child widgets GUM_TODO: make this an enum
    bool resizeWidgets;     // if true, child widgets will be resized to fit the container's width/height
    bool alignWidgets;      // if true, child widgets will be aligned automatically
    float padding;          // the space between the container's border and its child widgets
    float margin;           // the space between child widgets
GBL_INSTANCE_END

GBL_PROPERTIES(GUM_Container,
    (orientation,       GBL_GENERIC, (READ, WRITE), GBL_CHAR_TYPE),
    (resizeWidgets,     GBL_GENERIC, (READ, WRITE), GBL_BOOL_TYPE),
    (alignWidgets,      GBL_GENERIC, (READ, WRITE), GBL_BOOL_TYPE),
    (padding,           GBL_GENERIC, (READ, WRITE), GBL_FLOAT_TYPE),
    (margin,            GBL_GENERIC, (READ, WRITE), GBL_FLOAT_TYPE)
)

GblType GUM_Container_type(void);

// Takes a list of Name/Value pairs
#define GUM_Container_create(/* propertyName, propertyValue */ ...) GBL_NEW(GUM_Container __VA_OPT__(,) __VA_ARGS__)

GBL_DECLS_END
#undef GBL_SELF_TYPE

#endif // GUM_CONTAINER_H

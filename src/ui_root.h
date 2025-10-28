#ifndef UI_ROOT_H
#define UI_ROOT_H

#include <gimbal/gimbal_meta.h>

#define UI_ROOT_TYPE				(GBL_TYPEID(UI_Root))
#define UI_ROOT(self)				(GBL_CAST(UI_Root, self))
#define UI_ROOT_CLASS(klass)		(GBL_CLASS_CAST(UI_Root, klass))
#define UI_ROOT_CLASSOF(self)		(GBL_CLASSOF(UI_Root, self))

#define GBL_SELF_TYPE UI_Root

GBL_FORWARD_DECLARE_STRUCT(UI_Root);
GBL_DECLS_BEGIN

GBL_CLASS_DERIVE_EMPTY(UI_Root, GblObject)

GBL_INSTANCE_DERIVE_EMPTY(UI_Root, GblObject)

GblType UI_Root_type(void);

UI_Root* UI_Root_ref(GBL_SELF);

#define UI_Root_create(...) GBL_NEW(UI_Root __VA_OPT__(,) __VA_ARGS__)

GBL_DECLS_END
#undef GBL_SELF_TYPE

#endif // UI_ROOT_H

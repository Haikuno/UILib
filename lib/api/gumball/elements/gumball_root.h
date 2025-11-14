#ifndef GUM_ROOT_H
#define GUM_ROOT_H

#include <gimbal/gimbal_meta.h>
#include <gimbal/core/gimbal_module.h>
#include <gimbal/gimbal_containers.h>

#define GUM_ROOT_TYPE				(GBL_TYPEID(GUM_Root))
#define GUM_ROOT(self)				(GBL_CAST(GUM_Root, self))
#define GUM_ROOT_CLASS(klass)		(GBL_CLASS_CAST(GUM_Root, klass))
#define GUM_ROOT_CLASSOF(self)		(GBL_CLASSOF(GUM_Root, self))

#define GBL_SELF_TYPE GUM_Root

GBL_FORWARD_DECLARE_STRUCT(GUM_Root);
GBL_DECLS_BEGIN

GBL_CLASS_DERIVE_EMPTY(GUM_Root, GblModule)

GBL_INSTANCE_DERIVE_EMPTY(GUM_Root, GblModule)

GblType GUM_Root_type(void);

GUM_Root* GUM_Root_ref(GBL_SELF);

GUM_Root* GUM_Root_create(void);

// GUM_TODO: make these private later
void 			GUM_drawQueue_init(void);
void 			GUM_drawQueue_push(GblObject *pObj);
void 			GUM_drawQueue_remove(GblObject *pObj);
GblArrayList 	*GUM_drawQueue_get(void);

GBL_DECLS_END
#undef GBL_SELF_TYPE

#endif // GUM_ROOT_H

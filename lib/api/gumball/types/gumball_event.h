#ifndef GUM_EVENT_H
#define GUM_EVENT_H

#include <gimbal/gimbal_meta.h>

#define GUM_EVENT_TYPE				(GBL_TYPEID(GUM_Event))
#define GUM_EVENT(self)				(GBL_CAST(GUM_Event, self))
#define GUM_EVENT_CLASS(klass)		(GBL_CLASS_CAST(GUM_Event, klass))
#define GUM_EVENT_CLASSOF(self)		(GBL_CLASSOF(GUM_Event, self))

#define GBL_SELF_TYPE GUM_Event

GBL_FORWARD_DECLARE_STRUCT(GUM_Event);
GBL_DECLS_BEGIN

GBL_CLASS_DERIVE_EMPTY(GUM_Event, GblEvent)

GBL_INSTANCE_DERIVE_EMPTY(GUM_Event, GblEvent)

GblType GUM_Event_type(void);

GBL_DECLS_END
#undef GBL_SELF_TYPE

#endif // GUM_EVENT_H

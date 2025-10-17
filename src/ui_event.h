#ifndef UI_EVENT_H
#define UI_EVENT_H

#include <gimbal/gimbal_meta.h>

#define UI_EVENT_TYPE				(GBL_TYPEID(UI_Event))
#define UI_EVENT(self)				(GBL_CAST(UI_Event, self))
#define UI_EVENT_CLASS(klass)		(GBL_CLASS_CAST(UI_Event, klass))
#define UI_EVENT_GET_CLASS(self)	(GBL_CLASSOF(UI_Event, self))

#define GBL_SELF_TYPE UI_Event

GBL_FORWARD_DECLARE_STRUCT(UI_Event);
GBL_DECLS_BEGIN

GBL_CLASS_DERIVE_EMPTY(UI_Event, GblEvent)

GBL_INSTANCE_DERIVE_EMPTY(UI_Event, GblEvent)

GblType UI_Event_type(void);

GBL_DECLS_END
#undef GBL_SELF_TYPE

#endif // UI_EVENT_H

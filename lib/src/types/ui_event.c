#include <ui/types/ui_event.h>


GblType UI_Event_type(void) {
	static GblType type = GBL_INVALID_TYPE;

	if (type == GBL_INVALID_TYPE) {
		type = GblType_register(GblQuark_internStatic("UI_Event"),
							 	GBL_EVENT_TYPE,
							 	&(static GblTypeInfo){.classSize    = sizeof(UI_EventClass),
													  .instanceSize = sizeof(UI_Event)},
								GBL_TYPE_FLAG_TYPEINFO_STATIC);
	}

	return type;
}
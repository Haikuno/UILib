#include "ui_controller.h"
#include "ui_buttonevent.h"

static GBL_RESULT UI_ControllerClass_init_(GblClass *pClass, const void *pData) {
	GBL_UNUSED(pData);
	return GBL_RESULT_SUCCESS;
}

static GBL_RESULT UI_Controller_init_(GblInstance *pInstance) {
	return GBL_RESULT_SUCCESS;
}

void UI_Controller_notify_button(UI_Controller *pSelf, UI_CONTROLLER_BUTTON_STATE state, UI_CONTROLLER_BUTTON button) {
	UI_ButtonEvent *pEvent = UI_ButtonEvent_create(pSelf, state, button);
	GblObject_sendEvent(GBL_OBJECT(pSelf), GBL_EVENT(pEvent));
}

GblType UI_Controller_type(void) {
	static GblType type = GBL_INVALID_TYPE;

	if (type == GBL_INVALID_TYPE) {
		type =
			GblType_register(GblQuark_internStatic("UI_Controller"),
							 GBL_OBJECT_TYPE,
							 &(static GblTypeInfo){
													.classSize			= sizeof(UI_ControllerClass),
													.pFnClassInit		= UI_ControllerClass_init_,
													.instanceSize		= sizeof(UI_Controller),
													.pFnInstanceInit	= UI_Controller_init_ },
							 GBL_TYPE_FLAG_TYPEINFO_STATIC);
	}

	return type;
}
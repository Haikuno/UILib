#include "ui_root.h"
#include "ui_buttonevent.h"
#include "ui_widget.h"

static GBL_RESULT UI_Root_handle_event_(GblIEventHandler *pSelf, GblEvent *pEvent) {
	UI_ButtonEvent *button_event = (UI_ButtonEvent *)pEvent;

	printf("You %s %s!\n", button_event->state == UI_CONTROLLER_BUTTON_PRESS ? "pressed" : "released",
		button_event->button == UI_CONTROLLER_UP ? "up" :
		button_event->button == UI_CONTROLLER_RIGHT ? "right" :
		button_event->button == UI_CONTROLLER_DOWN ? "down" :
		button_event->button == UI_CONTROLLER_LEFT ? "left" :
		button_event->button == UI_CONTROLLER_PRIMARY ? "primary" :
		button_event->button == UI_CONTROLLER_SECONDARY ? "secondary" :
		button_event->button == UI_CONTROLLER_TERTIARY ? "tertiary" : "");

	GblEvent_accept(pEvent);
	return GBL_RESULT_SUCCESS;
}

static GBL_RESULT UI_RootClass_init_(GblClass *pClass, const void *pData) {
	GBL_UNUSED(pData);
	UI_ROOT_CLASS(pClass)->base.GblIEventHandlerImpl.pFnEvent = UI_Root_handle_event_;
	return GBL_RESULT_SUCCESS;
}

static GBL_RESULT UI_Root_init_(GblInstance *pInstance) {
	return GBL_RESULT_SUCCESS;
}

GblType UI_Root_type(void) {
	static GblType type = GBL_INVALID_TYPE;

	if (type == GBL_INVALID_TYPE) {
		type =
			GblType_register(GblQuark_internStatic("UI_Root"),
							 GBL_OBJECT_TYPE,
							 &(static GblTypeInfo){
													.classSize			= sizeof(UI_RootClass),
													.pFnClassInit		= UI_RootClass_init_,
													.instanceSize		= sizeof(UI_Root),
													.pFnInstanceInit	= UI_Root_init_ },
							 GBL_TYPE_FLAG_TYPEINFO_STATIC);
	}

	return type;
}

/// General UI functions ///

GBL_RESULT UI_update_(GblObject *pSelf) {
	size_t child_count = GblObject_childCount(pSelf);

	GblObjectClass	*pClass  = GBL_OBJECT_GET_CLASS(pSelf);
	UI_WidgetClass	*pClass_ = GBL_CLASS_AS(UI_Widget, pClass);

	if (pClass_ != nullptr) {
		pClass_->pFnUpdate(UI_WIDGET(pSelf));
	}

	for (size_t i = 0; i < child_count; i++) {
		GblObject *child_obj = GblObject_findChildByIndex(pSelf, i);
		UI_update_(child_obj);
	}

	return GBL_RESULT_SUCCESS;
}

GBL_RESULT UI_draw_(GblObject *pSelf) {
	size_t child_count = GblObject_childCount(pSelf);

	GblObjectClass	*pClass  = GBL_OBJECT_GET_CLASS(pSelf);
	UI_WidgetClass	*pClass_ = GBL_CLASS_AS(UI_Widget, pClass);

	if (pClass_ != nullptr) {
		pClass_->pFnDraw(UI_WIDGET(pSelf));
	}

	for (size_t i = 0; i < child_count; i++) {
		GblObject *child_obj = GblObject_findChildByIndex(pSelf, i);
		UI_draw_(child_obj);
	}

	return GBL_RESULT_SUCCESS;
}

GBL_RESULT UI_unref_(GblObject *pSelf) {
	size_t child_count = GblObject_childCount(pSelf);
	const char *name = GblType_name(GBL_TYPEOF(pSelf));

    for (size_t i = child_count; i-- > 0; ) {
        GblObject *child_obj = GblObject_findChildByIndex(pSelf, i);
        if (child_obj != nullptr) UI_unref_(child_obj);
    }

	GBL_UNREF(pSelf);

	return GBL_RESULT_SUCCESS;
}
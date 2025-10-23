#include "ui_controller.h"
#include "ui_buttonevent.h"

#include <raylib.h>


static GBL_RESULT UI_Controller_GblObject_setProperty_(GblObject *pObject, const GblProperty *pProp, GblVariant *pValue) {
	UI_Controller *pSelf = UI_CONTROLLER(pObject);

	switch (pProp->id) {
		case UI_Controller_Property_Id_controllerId:
			GblVariant_valueCopy(pValue, &pSelf->controllerId);
			break;
		case UI_Controller_Property_Id_isKeyboard:
			GblVariant_valueCopy(pValue, &pSelf->isKeyboard);
			break;
		default:
			return GBL_RESULT_ERROR_INVALID_PROPERTY;
	}

	return GBL_RESULT_SUCCESS;
}

static GBL_RESULT UI_Controller_GblObject_property_(const GblObject *pObject, const GblProperty *pProp, GblVariant *pValue) {
	UI_Controller *pSelf = UI_CONTROLLER(pObject);

	switch (pProp->id) {
		case UI_Controller_Property_Id_controllerId:
			GblVariant_setUint8(pValue, pSelf->controllerId);
			break;
		case UI_Controller_Property_Id_isKeyboard:
			GblVariant_setBool(pValue, pSelf->isKeyboard);
			break;
		default:
			return GBL_RESULT_ERROR_INVALID_PROPERTY;
	}

	return GBL_RESULT_SUCCESS;
}

static GBL_RESULT UI_Controller_update_(UI_Widget *pSelf) {
	UI_Controller *pController = UI_CONTROLLER(pSelf);

	//

	return GBL_RESULT_SUCCESS;
}

static GBL_RESULT UI_Controller_draw_(UI_Widget *pSelf) {
	UI_Controller *pController = UI_CONTROLLER(pSelf);

	if (pController->pSelectedButton) {
		Rectangle rec = {
			.x = UI_WIDGET(pController->pSelectedButton)->x,
			.y = UI_WIDGET(pController->pSelectedButton)->y,
			.width  = UI_WIDGET(pController->pSelectedButton)->w,
			.height = UI_WIDGET(pController->pSelectedButton)->h
		};

		DrawRectangleRoundedLinesEx(rec, pSelf->border_radius, 6, pSelf->border_width * 0.75, (Color){ pSelf->r, pSelf->g, pSelf->b, pSelf->a });
	}

	return GBL_RESULT_SUCCESS;
}

static GBL_RESULT UI_ControllerClass_init_(GblClass *pClass, const void *pData) {
	GBL_UNUSED(pData);

	if (!GblType_classRefCount(GBL_CLASS_TYPEOF(pClass))) GBL_PROPERTIES_REGISTER(UI_Controller);

	GBL_OBJECT_CLASS(pClass)->pFnSetProperty 	= UI_Controller_GblObject_setProperty_;
	GBL_OBJECT_CLASS(pClass)->pFnProperty    	= UI_Controller_GblObject_property_;
	UI_WIDGET_CLASS(pClass)->pFnDraw       		= UI_Controller_draw_;
	UI_WIDGET_CLASS(pClass)->pFnUpdate			= UI_Controller_update_;

	return GBL_RESULT_SUCCESS;
}

static GBL_RESULT UI_Controller_init_(GblInstance *pInstance) {
	UI_CONTROLLER(pInstance)->controllerId 		= 0;
	UI_CONTROLLER(pInstance)->isKeyboard 		= false;

	UI_WIDGET(pInstance)->r 					= 0;
	UI_WIDGET(pInstance)->g 					= 0;
	UI_WIDGET(pInstance)->b 					= 0;
	UI_WIDGET(pInstance)->a 					= 255;
	UI_WIDGET(pInstance)->z_index 				= 200;

	return GBL_RESULT_SUCCESS;
}

void UI_Controller_notifyButton(UI_Controller *pSelf, UI_CONTROLLER_BUTTON_STATE state, UI_CONTROLLER_BUTTON button) {
	UI_ButtonEvent *pEvent = UI_ButtonEvent_create(pSelf, state, button);
	GblObject_sendEvent(GBL_OBJECT(pSelf), GBL_EVENT(pEvent));
}

GblType UI_Controller_type(void) {
	static GblType type = GBL_INVALID_TYPE;

	if (type == GBL_INVALID_TYPE) {
		type =
			GblType_register(GblQuark_internStatic("UI_Controller"),
							 UI_WIDGET_TYPE,
							 &(static GblTypeInfo){
													.classSize			= sizeof(UI_ControllerClass),
													.pFnClassInit		= UI_ControllerClass_init_,
													.instanceSize		= sizeof(UI_Controller),
													.pFnInstanceInit	= UI_Controller_init_ },
							 GBL_TYPE_FLAG_TYPEINFO_STATIC);
	}

	return type;
}
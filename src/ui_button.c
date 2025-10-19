#include "ui_button.h"

static GBL_RESULT UI_Button_init_(GblInstance *pInstance) {
	UI_BUTTON(pInstance)->is_active = true;
	return GBL_RESULT_SUCCESS;
}

static GBL_RESULT UI_Button_draw_(UI_Widget *pSelf) {
	GBL_CTX_BEGIN(nullptr);
	if (!UI_BUTTON(pSelf)->is_active) {
		pSelf->a        = GBL_MIN(pSelf->a, 40);
		pSelf->border_a = GBL_MIN(pSelf->border_a, 40);
	}
	GBL_VCALL_DEFAULT(UI_Widget, pFnDraw, pSelf);
	GBL_CTX_END();
}

static GBL_RESULT UI_Button_GblObject_setProperty_(GblObject *pObject, const GblProperty *pProp, GblVariant *pValue) {
	UI_Button *pSelf = UI_BUTTON(pObject);
	switch (pProp->id) {
		case UI_Button_Property_Id_is_active:
			GblVariant_valueCopy(pValue, &pSelf->is_active);
			break;
		default:
			return GBL_RESULT_ERROR_INVALID_PROPERTY;
	}

	return GBL_RESULT_SUCCESS;
}

static GBL_RESULT UI_Button_GblObject_property_(const GblObject *pObject, const GblProperty *pProp, GblVariant *pValue) {
	UI_Button *pSelf = UI_BUTTON(pObject);

	switch (pProp->id) {
		case UI_Button_Property_Id_is_active:
			GblVariant_setBool(pValue, pSelf->is_active);
			break;
		default:
			return GBL_RESULT_ERROR_INVALID_PROPERTY;
	}

	return GBL_RESULT_SUCCESS;
}

static GBL_RESULT UI_ButtonClass_init_(GblClass *pClass, const void *pData) {
	GBL_UNUSED(pData);

	if (!GblType_classRefCount(GBL_CLASS_TYPEOF(pClass))) GBL_PROPERTIES_REGISTER(UI_Button);

	GblSignal_install(  UI_BUTTON_TYPE,
						"on_press",
						GblMarshal_CClosure_VOID__INSTANCE,
						0);

	GBL_OBJECT_CLASS(pClass)->pFnSetProperty = UI_Button_GblObject_setProperty_;
	GBL_OBJECT_CLASS(pClass)->pFnProperty    = UI_Button_GblObject_property_;

	UI_WIDGET_CLASS(pClass)->pFnActivate   = nullptr;
	UI_WIDGET_CLASS(pClass)->pFnDeactivate = nullptr;
	UI_WIDGET_CLASS(pClass)->pFnDraw       = UI_Button_draw_;

	return GBL_RESULT_SUCCESS;
}


GblType UI_Button_type(void) {
	static GblType type = GBL_INVALID_TYPE;

	if (type == GBL_INVALID_TYPE) {
		type =
			GblType_register(GblQuark_internStatic("UI_Button"),
							 UI_WIDGET_TYPE,
							 &(static GblTypeInfo){
													.classSize = sizeof(UI_ButtonClass),
													.pFnClassInit = UI_ButtonClass_init_,
													.instanceSize = sizeof(UI_Button),
													.pFnInstanceInit = UI_Button_init_ },
							 GBL_TYPE_FLAG_TYPEINFO_STATIC);

	}

	return type;
}

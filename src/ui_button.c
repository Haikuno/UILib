#include "ui_button.h"

static GBL_RESULT UI_Button_init_(GblInstance *pInstance) {
	UI_BUTTON(pInstance)->isActive		= true;
	UI_BUTTON(pInstance)->isSelectable	= true;
	UI_BUTTON(pInstance)->isSelected	= false;
	return GBL_RESULT_SUCCESS;
}

static GBL_RESULT UI_Button_draw_(UI_Widget *pSelf) {
	GBL_CTX_BEGIN(nullptr);
	if (!UI_BUTTON(pSelf)->isActive) {
		pSelf->a        = GBL_MIN(pSelf->a, 40);
		pSelf->border_a = GBL_MIN(pSelf->border_a, 40);
	}

	if (UI_BUTTON(pSelf)->isSelected) {
		pSelf->border_a        = 200;
		pSelf->border_r        = 255;
		pSelf->border_g        = 255;
		pSelf->border_b        = 255;
		pSelf->border_width	   = 2;
	} else {
		pSelf->border_a = 0;
	}

	GBL_VCALL_DEFAULT(UI_Widget, pFnDraw, pSelf);
	GBL_CTX_END();
}

static GBL_RESULT UI_Button_GblObject_setProperty_(GblObject *pObject, const GblProperty *pProp, GblVariant *pValue) {
	UI_Button *pSelf = UI_BUTTON(pObject);
	switch (pProp->id) {
		case UI_Button_Property_Id_isActive:
			GblVariant_valueCopy(pValue, &pSelf->isActive);
			break;
		case UI_Button_Property_Id_isSelectable:
			GblVariant_valueCopy(pValue, &pSelf->isSelectable);
			break;
		default:
			return GBL_RESULT_ERROR_INVALID_PROPERTY;
	}

	return GBL_RESULT_SUCCESS;
}

static GBL_RESULT UI_Button_GblObject_property_(const GblObject *pObject, const GblProperty *pProp, GblVariant *pValue) {
	UI_Button *pSelf = UI_BUTTON(pObject);

	switch (pProp->id) {
		case UI_Button_Property_Id_isActive:
			GblVariant_setBool(pValue, pSelf->isActive);
			break;
		case UI_Button_Property_Id_isSelectable:
			GblVariant_setBool(pValue, pSelf->isSelectable);
			break;
		default:
			return GBL_RESULT_ERROR_INVALID_PROPERTY;
	}

	return GBL_RESULT_SUCCESS;
}

static GBL_RESULT UI_ButtonClass_init_(GblClass *pClass, const void *pData) {
	GBL_UNUSED(pData);

	if (!GblType_classRefCount(GBL_CLASS_TYPEOF(pClass))) {
		GBL_PROPERTIES_REGISTER(UI_Button);

		GblSignal_install(  UI_BUTTON_TYPE,
							"onPressPrimary",
							GblMarshal_CClosure_VOID__INSTANCE,
							0);

		GblSignal_install(  UI_BUTTON_TYPE,
							"onPressSecondary",
							GblMarshal_CClosure_VOID__INSTANCE,
							0);

		GblSignal_install(  UI_BUTTON_TYPE,
							"onPressTertiary",
							GblMarshal_CClosure_VOID__INSTANCE,
							0);
	}

	GBL_OBJECT_CLASS(pClass)->pFnSetProperty = UI_Button_GblObject_setProperty_;
	GBL_OBJECT_CLASS(pClass)->pFnProperty    = UI_Button_GblObject_property_;

	UI_WIDGET_CLASS(pClass)->pFnActivate   = nullptr;
	UI_WIDGET_CLASS(pClass)->pFnDeactivate = nullptr;
	UI_WIDGET_CLASS(pClass)->pFnDraw       = UI_Button_draw_;

	return GBL_RESULT_SUCCESS;
}

static GBL_RESULT UI_ButtonClass_final_(GblClass* pClass, const void* pClassData) {
    GBL_UNUSED(pClassData);

    if(!GblType_classRefCount(GBL_CLASS_TYPEOF(pClass))) {
        GblSignal_uninstall(UI_BUTTON_TYPE, "onPressPrimary");
		GblSignal_uninstall(UI_BUTTON_TYPE, "onPressSecondary");
		GblSignal_uninstall(UI_BUTTON_TYPE, "onPressTertiary");
    }
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
													.pFnInstanceInit = UI_Button_init_,
													.pFnClassFinal = UI_ButtonClass_final_ },
							 GBL_TYPE_FLAG_TYPEINFO_STATIC);

	}

	return type;
}

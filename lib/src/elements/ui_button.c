#include <ui/elements/ui_button.h>
#include <ui/elements/ui_controller.h>
#include <ui/elements/ui_root.h>

static GBL_RESULT UI_Button_init_(GblInstance *pInstance) {
	UI_Button *pButton = UI_BUTTON(pInstance);

	pButton->isActive				= true;
	pButton->isSelectable			= true;
	pButton->isSelected				= false;
	pButton->isSelectedByDefault	= false;

	return GBL_RESULT_SUCCESS;
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
		case UI_Button_Property_Id_isSelectedByDefault:
			GblVariant_valueCopy(pValue, &pSelf->isSelectedByDefault);
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
		case UI_Button_Property_Id_isSelectedByDefault:
			GblVariant_setBool(pValue, pSelf->isSelectedByDefault);
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

	return GBL_RESULT_SUCCESS;
}

static GBL_RESULT UI_ButtonClass_final_(GblClass* pClass, const void* pClassData) {
    GBL_UNUSED(pClassData);

    if(!GblType_classRefCount(UI_BUTTON_TYPE)) {
        GblSignal_uninstall(UI_BUTTON_TYPE, "onPressPrimary");
		GblSignal_uninstall(UI_BUTTON_TYPE, "onPressSecondary");
		GblSignal_uninstall(UI_BUTTON_TYPE, "onPressTertiary");
    }

	return GBL_RESULT_SUCCESS;
}

GblType UI_Button_type(void) {
	static GblType type = GBL_INVALID_TYPE;

	if (type == GBL_INVALID_TYPE) {
		type = GblType_register(GblQuark_internStatic("UI_Button"),
								UI_WIDGET_TYPE,
								&(static GblTypeInfo){.classSize = sizeof(UI_ButtonClass),
													  .pFnClassInit = UI_ButtonClass_init_,
													  .instanceSize = sizeof(UI_Button),
													  .pFnInstanceInit = UI_Button_init_,
													  .pFnClassFinal = UI_ButtonClass_final_},
								GBL_TYPE_FLAG_TYPEINFO_STATIC);

	}

	return type;
}
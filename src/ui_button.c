#include "ui_button.h"

static GBL_RESULT UI_Button_init_(GblInstance *pInstance) {
    UI_BUTTON(pInstance)->is_active = true;
    return GBL_RESULT_SUCCESS;
}

static GBL_RESULT UI_Button_draw_(UI_Widget *pSelf) {
    GBL_CTX_BEGIN(NULL);
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

    GBL_OBJECT_CLASS(pClass)->pFnSetProperty = UI_Button_GblObject_setProperty_;
    GBL_OBJECT_CLASS(pClass)->pFnProperty    = UI_Button_GblObject_property_;

    UI_WIDGET_CLASS(pClass)->pFnActivate   = NULL;
    UI_WIDGET_CLASS(pClass)->pFnDeactivate = NULL;
    UI_WIDGET_CLASS(pClass)->pFnDraw       = UI_Button_draw_;

    return GBL_RESULT_SUCCESS;
}


GblType UI_Button_type(void) {
    // Initialize our type as not registered
    static GblType type = GBL_INVALID_TYPE;

    // If it's our fist time calling, we have no UUID
    if (type == GBL_INVALID_TYPE) {
        // Register the type with libGimbal and store its UUID for later
        type = // Get fast-ass interned string for name of type
            GblType_register(GblQuark_internStatic("UI_Button"),
                             // Parent type you're inheriting from
                             UI_WIDGET_TYPE,
                             // Type information structure
                             &(static GblTypeInfo){ // Size of your static members + virtaul methods
                                                    .classSize = sizeof(UI_ButtonClass),
                                                    // Class "construtor"
                                                    .pFnClassInit = UI_ButtonClass_init_,
                                                    // Size of every instance of the class
                                                    .instanceSize = sizeof(UI_Widget),
                                                    // Constructor for each instance
                                                    .pFnInstanceInit = UI_Button_init_ },
                             // Any additional flags
                             GBL_TYPE_FLAG_TYPEINFO_STATIC);
    }

    // Return its UUID
    return type;
}

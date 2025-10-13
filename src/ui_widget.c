#include "ui_widget.h"

#include <raylib.h>

static GBL_RESULT UIWidget_init_(GblInstance *pInstance) {
    UI_WIDGET(pInstance)->r = 0;
    UI_WIDGET(pInstance)->g = 255;
    UI_WIDGET(pInstance)->b = 0;
    UI_WIDGET(pInstance)->a = 255;

    UI_WIDGET(pInstance)->x = 160;
    UI_WIDGET(pInstance)->y = 120;

    UI_WIDGET(pInstance)->w = 320;
    UI_WIDGET(pInstance)->h = 240;

    return GBL_RESULT_SUCCESS;
}

static GBL_RESULT UIWidget_GblObject_setProperty_(GblObject *pObject, const GblProperty *pProp, GblVariant *pValue) {
    UIWidget *pSelf = UI_WIDGET(pObject);

    switch (pProp->id) {
        case UIWidget_Property_Id_x:
            GblVariant_valueCopy(pValue, &pSelf->x);
            break;
        case UIWidget_Property_Id_y:
            GblVariant_valueCopy(pValue, &pSelf->y);
            break;
        case UIWidget_Property_Id_w:
            GblVariant_valueCopy(pValue, &pSelf->w);
            break;
        case UIWidget_Property_Id_h:
            GblVariant_valueCopy(pValue, &pSelf->h);
            break;
        case UIWidget_Property_Id_color:
            uint32_t color_;
            GblVariant_valueCopy(pValue, &color_);
            pSelf->r = (color_ >> 24) & 0xFF;
            pSelf->g = (color_ >> 16) & 0xFF;
            pSelf->b = (color_ >> 8) & 0xFF;
            pSelf->a = color_ & 0xFF;
            break;
        case UIWidget_Property_Id_r:
            GblVariant_valueCopy(pValue, &pSelf->r);
            break;
        case UIWidget_Property_Id_g:
            GblVariant_valueCopy(pValue, &pSelf->g);
            break;
        case UIWidget_Property_Id_b:
            GblVariant_valueCopy(pValue, &pSelf->b);
            break;
        case UIWidget_Property_Id_a:
            GblVariant_valueCopy(pValue, &pSelf->a);
            break;
        default:
            return GBL_RESULT_ERROR_INVALID_PROPERTY;
    }

    return GBL_RESULT_SUCCESS;
}

static GBL_RESULT UIWidget_GblObject_property_(const GblObject *pObject, const GblProperty *pProp, GblVariant *pValue) {
    UIWidget *pSelf = UI_WIDGET(pObject);

    switch (pProp->id) {
        case UIWidget_Property_Id_x:
            GblVariant_setFloat(pValue, pSelf->x);
            break;
        case UIWidget_Property_Id_y:
            GblVariant_setFloat(pValue, pSelf->y);
            break;
        case UIWidget_Property_Id_w:
            GblVariant_setFloat(pValue, pSelf->w);
            break;
        case UIWidget_Property_Id_h:
            GblVariant_setFloat(pValue, pSelf->h);
            break;
        case UIWidget_Property_Id_color:
            GblVariant_setUint32(pValue, pSelf->r << 24 | pSelf->g << 16 | pSelf->b << 8 | pSelf->a);
            break;
        case UIWidget_Property_Id_r:
            GblVariant_setUint8(pValue, pSelf->r);
            break;
        case UIWidget_Property_Id_g:
            GblVariant_setUint8(pValue, pSelf->g);
            break;
        case UIWidget_Property_Id_b:
            GblVariant_setUint8(pValue, pSelf->b);
            break;
        case UIWidget_Property_Id_a:
            GblVariant_setUint8(pValue, pSelf->a);
            break;
        default:
            return GBL_RESULT_ERROR_INVALID_PROPERTY;
    }

    return GBL_RESULT_SUCCESS;
}

static void UIWidget_draw_(UIWidget *pSelf) {
    DrawRectangle((int)pSelf->x, (int)pSelf->y, (int)pSelf->w, (int)pSelf->h, (Color){ pSelf->r, pSelf->g, pSelf->b, pSelf->a });
}

static GBL_RESULT UIWidgetClass_init_(GblClass *pClass, const void *pData) {
    GBL_UNUSED(pData);
    // Check if this is the first instance of the class
    if (!GblType_classRefCount(GBL_CLASS_TYPEOF(pClass))) GBL_PROPERTIES_REGISTER(UIWidget);

    GBL_OBJECT_CLASS(pClass)->pFnSetProperty = UIWidget_GblObject_setProperty_;
    GBL_OBJECT_CLASS(pClass)->pFnProperty    = UIWidget_GblObject_property_;

    UI_WIDGET_CLASS(pClass)->pFnActivate   = NULL;
    UI_WIDGET_CLASS(pClass)->pFnDeactivate = NULL;
    UI_WIDGET_CLASS(pClass)->pFnDraw       = UIWidget_draw_;

    return GBL_RESULT_SUCCESS;
}

GblType UIWidget_type(void) {
    // Initialize our type as not registered
    static GblType type = GBL_INVALID_TYPE;

    // If it's our fist time calling, we have no UUID
    if (type == GBL_INVALID_TYPE) {
        // Register the type with libGimbal and store its UUID for later
        type = // Get fast-ass interned string for name of type
            GblType_register(GblQuark_internStatic("UIWidget"),
                             // Parent type you're inheriting from
                             GBL_OBJECT_TYPE,
                             // Type information structure
                             &(static GblTypeInfo){ // Size of your static members + virtaul methods
                                                    .classSize = sizeof(UIWidgetClass),
                                                    // Class "construtor"
                                                    .pFnClassInit = UIWidgetClass_init_,
                                                    // Size of every instance of the class
                                                    .instanceSize = sizeof(UIWidget),
                                                    // Constructor for each instance
                                                    .pFnInstanceInit = UIWidget_init_ },
                             // Any additional flags
                             GBL_TYPE_FLAG_TYPEINFO_STATIC);
    }

    // Return its UUID
    return type;
}

GBL_RESULT UIWidget_draw(UIWidget *pSelf) {
    if (!GblType_check(GBL_TYPEOF(pSelf), UI_WIDGET_TYPE)) {
        return GBL_INVALID_TYPE;
    }
    UI_WIDGET_GET_CLASS(pSelf)->pFnDraw(pSelf);
    return GBL_RESULT_SUCCESS;
}

UIWidget *UIWidget_ref(UIWidget* widget) {
    return UI_WIDGET(GBL_REF(widget));
}

void UIWidget_unref(UIWidget* widget) {
    GBL_UNREF(widget);
}
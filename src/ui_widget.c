#include "ui_widget.h"

#include <raylib.h>

static GBL_RESULT UI_Widget_init_(GblInstance *pInstance) {
    UI_WIDGET(pInstance)->r = 0;
    UI_WIDGET(pInstance)->g = 255;
    UI_WIDGET(pInstance)->b = 0;
    UI_WIDGET(pInstance)->a = 255;

    UI_WIDGET(pInstance)->border_r     = 0;
    UI_WIDGET(pInstance)->border_g     = 0;
    UI_WIDGET(pInstance)->border_b     = 0;
    UI_WIDGET(pInstance)->border_a     = 0;
    UI_WIDGET(pInstance)->border_width = 2;

    UI_WIDGET(pInstance)->x = 160;
    UI_WIDGET(pInstance)->y = 120;

    UI_WIDGET(pInstance)->w = 320;
    UI_WIDGET(pInstance)->h = 240;

    UI_WIDGET(pInstance)->is_relative = true;

    return GBL_RESULT_SUCCESS;
}

static GBL_RESULT UI_Widget_GblObject_setProperty_(GblObject *pObject, const GblProperty *pProp, GblVariant *pValue) {
    UI_Widget *pSelf = UI_WIDGET(pObject);

    switch (pProp->id) {
        case UI_Widget_Property_Id_x:
            GblVariant_valueCopy(pValue, &pSelf->x);
            break;
        case UI_Widget_Property_Id_y:
            GblVariant_valueCopy(pValue, &pSelf->y);
            break;
        case UI_Widget_Property_Id_w:
            GblVariant_valueCopy(pValue, &pSelf->w);
            break;
        case UI_Widget_Property_Id_h:
            GblVariant_valueCopy(pValue, &pSelf->h);
            break;
        case UI_Widget_Property_Id_is_relative:
            GblVariant_valueCopy(pValue, &pSelf->is_relative);
            break;
        case UI_Widget_Property_Id_color:
            uint32_t color_;
            GblVariant_valueCopy(pValue, &color_);
            pSelf->r = (color_ >> 24) & 0xFF;
            pSelf->g = (color_ >> 16) & 0xFF;
            pSelf->b = (color_ >> 8) & 0xFF;
            pSelf->a = color_ & 0xFF;
            break;
        case UI_Widget_Property_Id_border_color:
            uint32_t border_color_;
            GblVariant_valueCopy(pValue, &border_color_);
            pSelf->border_r = (border_color_ >> 24) & 0xFF;
            pSelf->border_g = (border_color_ >> 16) & 0xFF;
            pSelf->border_b = (border_color_ >> 8) & 0xFF;
            pSelf->border_a = border_color_ & 0xFF;
            break;
        case UI_Widget_Property_Id_r:
            GblVariant_valueCopy(pValue, &pSelf->r);
            break;
        case UI_Widget_Property_Id_g:
            GblVariant_valueCopy(pValue, &pSelf->g);
            break;
        case UI_Widget_Property_Id_b:
            GblVariant_valueCopy(pValue, &pSelf->b);
            break;
        case UI_Widget_Property_Id_a:
            GblVariant_valueCopy(pValue, &pSelf->a);
            break;
        case UI_Widget_Property_Id_border_r:
            GblVariant_valueCopy(pValue, &pSelf->border_r);
            break;
        case UI_Widget_Property_Id_border_g:
            GblVariant_valueCopy(pValue, &pSelf->border_g);
            break;
        case UI_Widget_Property_Id_border_b:
            GblVariant_valueCopy(pValue, &pSelf->border_b);
            break;
        case UI_Widget_Property_Id_border_a:
            GblVariant_valueCopy(pValue, &pSelf->border_a);
            break;
        default:
            return GBL_RESULT_ERROR_INVALID_PROPERTY;
    }

    return GBL_RESULT_SUCCESS;
}

static GBL_RESULT UI_Widget_GblObject_property_(const GblObject *pObject, const GblProperty *pProp, GblVariant *pValue) {
    UI_Widget *pSelf = UI_WIDGET(pObject);

    switch (pProp->id) {
        case UI_Widget_Property_Id_x:
            GblVariant_setFloat(pValue, pSelf->x);
            break;
        case UI_Widget_Property_Id_y:
            GblVariant_setFloat(pValue, pSelf->y);
            break;
        case UI_Widget_Property_Id_w:
            GblVariant_setFloat(pValue, pSelf->w);
            break;
        case UI_Widget_Property_Id_h:
            GblVariant_setFloat(pValue, pSelf->h);
            break;
        case UI_Widget_Property_Id_is_relative:
            GblVariant_setBool(pValue, pSelf->is_relative);
            break;
        case UI_Widget_Property_Id_color:
            GblVariant_setUint32(pValue, pSelf->r << 24 | pSelf->g << 16 | pSelf->b << 8 | pSelf->a);
            break;
        case UI_Widget_Property_Id_border_color:
            GblVariant_setUint32(pValue, pSelf->border_r << 24 | pSelf->border_g << 16 | pSelf->border_b << 8 | pSelf->border_a);
            break;
        case UI_Widget_Property_Id_r:
            GblVariant_setUint8(pValue, pSelf->r);
            break;
        case UI_Widget_Property_Id_g:
            GblVariant_setUint8(pValue, pSelf->g);
            break;
        case UI_Widget_Property_Id_b:
            GblVariant_setUint8(pValue, pSelf->b);
            break;
        case UI_Widget_Property_Id_a:
            GblVariant_setUint8(pValue, pSelf->a);
            break;
        case UI_Widget_Property_Id_border_r:
            GblVariant_setUint8(pValue, pSelf->border_r);
            break;
        case UI_Widget_Property_Id_border_g:
            GblVariant_setUint8(pValue, pSelf->border_g);
            break;
        case UI_Widget_Property_Id_border_b:
            GblVariant_setUint8(pValue, pSelf->border_b);
            break;
        case UI_Widget_Property_Id_border_a:
            GblVariant_setUint8(pValue, pSelf->border_a);
            break;
        default:
            return GBL_RESULT_ERROR_INVALID_PROPERTY;
    }

    return GBL_RESULT_SUCCESS;
}

static void UI_Widget_draw_(UI_Widget *pSelf) {
    Rectangle  rec    = (Rectangle){ pSelf->x, pSelf->y, pSelf->w, pSelf->h };
    GblObject *parent = GblObject_parent(GBL_OBJECT(pSelf));

    if (parent && pSelf->is_relative) {
        rec.x += UI_WIDGET(parent)->x;
        rec.y += UI_WIDGET(parent)->y;
    }

    if (pSelf->a) {
        DrawRectangleRec(rec, (Color){ pSelf->r, pSelf->g, pSelf->b, pSelf->a });
    }
    if (pSelf->border_a) {
        DrawRectangleLinesEx(rec, pSelf->border_width, (Color){ pSelf->border_r, pSelf->border_g, pSelf->border_b, pSelf->border_a });
    }
}

static GBL_RESULT UI_WidgetClass_init_(GblClass *pClass, const void *pData) {
    GBL_UNUSED(pData);
    // Check if this is the first instance of the class
    if (!GblType_classRefCount(GBL_CLASS_TYPEOF(pClass))) GBL_PROPERTIES_REGISTER(UI_Widget);

    GBL_OBJECT_CLASS(pClass)->pFnSetProperty = UI_Widget_GblObject_setProperty_;
    GBL_OBJECT_CLASS(pClass)->pFnProperty    = UI_Widget_GblObject_property_;

    UI_WIDGET_CLASS(pClass)->pFnActivate   = NULL;
    UI_WIDGET_CLASS(pClass)->pFnDeactivate = NULL;
    UI_WIDGET_CLASS(pClass)->pFnDraw       = UI_Widget_draw_;

    return GBL_RESULT_SUCCESS;
}

GblType UI_Widget_type(void) {
    // Initialize our type as not registered
    static GblType type = GBL_INVALID_TYPE;

    // If it's our fist time calling, we have no UUID
    if (type == GBL_INVALID_TYPE) {
        // Register the type with libGimbal and store its UUID for later
        type = // Get fast-ass interned string for name of type
            GblType_register(GblQuark_internStatic("UI_Widget"),
                             // Parent type you're inheriting from
                             GBL_OBJECT_TYPE,
                             // Type information structure
                             &(static GblTypeInfo){ // Size of your static members + virtaul methods
                                                    .classSize = sizeof(UI_WidgetClass),
                                                    // Class "construtor"
                                                    .pFnClassInit = UI_WidgetClass_init_,
                                                    // Size of every instance of the class
                                                    .instanceSize = sizeof(UI_Widget),
                                                    // Constructor for each instance
                                                    .pFnInstanceInit = UI_Widget_init_ },
                             // Any additional flags
                             GBL_TYPE_FLAG_TYPEINFO_STATIC);
    }

    // Return its UUID
    return type;
}

void UI_make_child(UI_Widget *pParent, UI_Widget *pChild) {
    GblObject_setParent(GBL_OBJECT(pChild), GBL_OBJECT(pParent));
}

GBL_RESULT UI_draw(UI_Widget *pSelf) {
    if (!GblType_check(GBL_TYPEOF(pSelf), UI_WIDGET_TYPE)) {
        return GBL_INVALID_TYPE;
    }

    UI_WIDGET_GET_CLASS(pSelf)->pFnDraw(pSelf);

    size_t child_count = GblObject_childCount(GBL_OBJECT(pSelf));

    for (size_t i = 0; i < child_count; ++i) {
        GblObject *child_obj = GblObject_findChildByIndex(GBL_OBJECT(pSelf), i);
        UI_draw(UI_WIDGET(child_obj));
    }

    return GBL_RESULT_SUCCESS;
}

UI_Widget *UI_Widget_ref(UI_Widget *pSelf) {
    return UI_WIDGET(GBL_REF(pSelf));
}

void UI_unref(UI_Widget *pSelf) {
    size_t child_count = GblObject_childCount(GBL_OBJECT(pSelf));

    for (size_t i = 0; i < child_count; ++i) {
        GblObject *child_obj = GblObject_findChildByIndex(GBL_OBJECT(pSelf), i);
        UI_unref(UI_WIDGET(child_obj));
    }

    GBL_UNREF(pSelf);
}

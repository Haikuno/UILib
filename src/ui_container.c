#include "ui_container.h"
#include "ui_widget.h"

static GBL_RESULT UI_Container_init_(GblInstance *pInstance) {
    UI_CONTAINER(pInstance)->resize_widgets = true;
    UI_CONTAINER(pInstance)->align_widgets  = true;
    UI_CONTAINER(pInstance)->padding        = 5.0f;
    UI_CONTAINER(pInstance)->margin         = 5.0f;
    return GBL_RESULT_SUCCESS;
}

static GBL_RESULT UI_Container_update_(UI_Widget *pSelf) {
    size_t child_count = GblObject_childCount(GBL_OBJECT(pSelf));
    if (child_count == 0) return GBL_RESULT_SUCCESS;

	const bool 	is_horizontal 				= UI_CONTAINER(pSelf)->orientation == 'h' || UI_CONTAINER(pSelf)->orientation == 'H';
    const float total_margin  				= UI_CONTAINER(pSelf)->margin * (float)(child_count + 1);
    const float total_padding 				= UI_CONTAINER(pSelf)->padding * 2.0f;
	const float container_main_pos 			= is_horizontal ? pSelf->x : pSelf->y;
	const float container_secondary_pos		= is_horizontal ? pSelf->y : pSelf->x;
	const float container_main_dim			= is_horizontal ? pSelf->w : pSelf->h;
	const float container_secondary_dim		= is_horizontal ? pSelf->h : pSelf->w;
	float offset 							= container_main_pos + UI_CONTAINER(pSelf)->padding;

	for (size_t i = 0; i < child_count; ++i) {
		GblObject *child_obj    = GblObject_findChildByIndex(GBL_OBJECT(pSelf), i);
		UI_Widget *child_widget = UI_WIDGET(child_obj);

		float *widget_main_pos		= is_horizontal ? &child_widget->x : &child_widget->y;
		float *widget_secondary_pos = is_horizontal ? &child_widget->y : &child_widget->x;
		float *widget_main_dim		= is_horizontal ? &child_widget->w : &child_widget->h;
		float *widget_secondary_dim = is_horizontal ? &child_widget->h : &child_widget->w;

		if (UI_CONTAINER(pSelf)->resize_widgets) {
			*widget_main_dim		= (container_main_dim - total_margin - total_padding) / (float)child_count;
			*widget_secondary_dim	= container_secondary_dim - total_padding;
		}

		if (UI_CONTAINER(pSelf)->align_widgets) {
			if (i == 0) offset += UI_CONTAINER(pSelf)->margin;

			*widget_main_pos		= offset;
			*widget_secondary_pos	= container_secondary_pos + UI_CONTAINER(pSelf)->padding;
			offset 					+= *widget_main_dim + UI_CONTAINER(pSelf)->margin;
		}
	}
}

static GBL_RESULT UI_Container_draw_(UI_Widget *pSelf) {
    GBL_CTX_BEGIN(NULL);
    GBL_VCALL_DEFAULT(UI_Widget, pFnDraw, pSelf);
    GBL_CTX_END();
}

static GBL_RESULT UI_Container_GblObject_setProperty_(GblObject *pObject, const GblProperty *pProp, GblVariant *pValue) {
    UI_Container *pSelf = UI_CONTAINER(pObject);
    switch (pProp->id) {
        case UI_Container_Property_Id_orientation:
            GblVariant_valueCopy(pValue, &pSelf->orientation);
            break;
        case UI_Container_Property_Id_resize_widgets:
            GblVariant_valueCopy(pValue, &pSelf->resize_widgets);
            break;
        case UI_Container_Property_Id_align_widgets:
            GblVariant_valueCopy(pValue, &pSelf->align_widgets);
            break;
        case UI_Container_Property_Id_padding:
            GblVariant_valueCopy(pValue, &pSelf->padding);
            break;
        case UI_Container_Property_Id_margin:
            GblVariant_valueCopy(pValue, &pSelf->margin);
            break;
        default:
            return GBL_RESULT_ERROR_INVALID_PROPERTY;
    }

    return GBL_RESULT_SUCCESS;
}

static GBL_RESULT UI_Container_GblObject_property_(const GblObject *pObject, const GblProperty *pProp, GblVariant *pValue) {
    UI_Container *pSelf = UI_CONTAINER(pObject);

    switch (pProp->id) {
        case UI_Container_Property_Id_orientation:
            GblVariant_setChar(pValue, pSelf->orientation);
            break;
        case UI_Container_Property_Id_resize_widgets:
            GblVariant_setBool(pValue, pSelf->resize_widgets);
            break;
        case UI_Container_Property_Id_align_widgets:
            GblVariant_setBool(pValue, pSelf->align_widgets);
            break;
        case UI_Container_Property_Id_padding:
            GblVariant_setFloat(pValue, pSelf->padding);
            break;
        case UI_Container_Property_Id_margin:
            GblVariant_setFloat(pValue, pSelf->margin);
            break;
        default:
            return GBL_RESULT_ERROR_INVALID_PROPERTY;
    }

    return GBL_RESULT_SUCCESS;
}

static GBL_RESULT UI_ContainerClass_init_(GblClass *pClass, const void *pData) {
    GBL_UNUSED(pData);

    if (!GblType_classRefCount(GBL_CLASS_TYPEOF(pClass))) GBL_PROPERTIES_REGISTER(UI_Container);

    GBL_OBJECT_CLASS(pClass)->pFnSetProperty = UI_Container_GblObject_setProperty_;
    GBL_OBJECT_CLASS(pClass)->pFnProperty    = UI_Container_GblObject_property_;

    UI_WIDGET_CLASS(pClass)->pFnActivate   = NULL;
    UI_WIDGET_CLASS(pClass)->pFnDeactivate = NULL;
    UI_WIDGET_CLASS(pClass)->pFnUpdate     = UI_Container_update_;
    UI_WIDGET_CLASS(pClass)->pFnDraw       = UI_Container_draw_;

    return GBL_RESULT_SUCCESS;
}

GblType UI_Container_type(void) {
    // Initialize our type as not registered
    static GblType type = GBL_INVALID_TYPE;

    // If it's our fist time calling, we have no UUID
    if (type == GBL_INVALID_TYPE) {
        // Register the type with libGimbal and store its UUID for later
        type = // Get fast-ass interned string for name of type
            GblType_register(GblQuark_internStatic("UI_Container"),
                             // Parent type you're inheriting from
                             UI_WIDGET_TYPE,
                             // Type information structure
                             &(static GblTypeInfo){ // Size of your static members + virtaul methods
                                                    .classSize = sizeof(UI_ContainerClass),
                                                    // Class "construtor"
                                                    .pFnClassInit = UI_ContainerClass_init_,
                                                    // Size of every instance of the class
                                                    .instanceSize = sizeof(UI_Container),
                                                    // Constructor for each instance
                                                    .pFnInstanceInit = UI_Container_init_ },
                             // Any additional flags
                             GBL_TYPE_FLAG_TYPEINFO_STATIC);
    }

    // Return its UUID
    return type;
}

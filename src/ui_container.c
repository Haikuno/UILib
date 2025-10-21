#include "ui_container.h"
#include "ui_widget.h"

static GBL_RESULT UI_Container_init_(GblInstance *pInstance) {
    UI_CONTAINER(pInstance)->resizeWidgets = true;
    UI_CONTAINER(pInstance)->alignWidgets  = true;
    UI_CONTAINER(pInstance)->padding        = 5.0f;
    UI_CONTAINER(pInstance)->margin         = 5.0f;
    UI_CONTAINER(pInstance)->orientation    = 'v';
    return GBL_RESULT_SUCCESS;
}

static GBL_RESULT UI_Container_update_(UI_Widget *pSelf) {
    size_t childCount = GblObject_childCount(GBL_OBJECT(pSelf));
    if (childCount == 0) return GBL_RESULT_SUCCESS;

	const bool 	isHorizontal 				= UI_CONTAINER(pSelf)->orientation == 'h' || UI_CONTAINER(pSelf)->orientation == 'H';
    const float totalMargin  				= UI_CONTAINER(pSelf)->margin * (float)(childCount + 1);
    const float totalPadding 				= UI_CONTAINER(pSelf)->padding * 2.0f;

	const float container_mainPos 			= isHorizontal ? pSelf->x : pSelf->y;
	const float container_secondaryPos		= isHorizontal ? pSelf->y : pSelf->x;
	const float container_mainDim			= isHorizontal ? pSelf->w : pSelf->h;
	const float container_secondaryDim		= isHorizontal ? pSelf->h : pSelf->w;

	float offset 							= container_mainPos + UI_CONTAINER(pSelf)->padding;

	for (size_t i = 0; i < childCount; ++i) {
		GblObject *child_obj    = GblObject_findChildByIndex(GBL_OBJECT(pSelf), i);
		UI_Widget *child_widget = UI_WIDGET(child_obj);

		float *widget_mainPos		= isHorizontal ? &child_widget->x : &child_widget->y;
		float *widget_secondaryPos  = isHorizontal ? &child_widget->y : &child_widget->x;
		float *widget_mainDim		= isHorizontal ? &child_widget->w : &child_widget->h;
		float *widget_secondaryDim  = isHorizontal ? &child_widget->h : &child_widget->w;

		if (UI_CONTAINER(pSelf)->resizeWidgets) {
			*widget_mainDim		    = (container_mainDim - totalMargin - totalPadding) / (float)childCount;
			*widget_secondaryDim	= container_secondaryDim - totalPadding;
		}

		if (UI_CONTAINER(pSelf)->alignWidgets) {
			if (i == 0) offset += UI_CONTAINER(pSelf)->margin;

			*widget_mainPos		    = offset;
			*widget_secondaryPos	= container_secondaryPos + UI_CONTAINER(pSelf)->padding;
			offset 					+= *widget_mainDim + UI_CONTAINER(pSelf)->margin;
		}
	}

    return GBL_RESULT_SUCCESS;
}

static GBL_RESULT UI_Container_GblObject_setProperty_(GblObject *pObject, const GblProperty *pProp, GblVariant *pValue) {
    UI_Container *pSelf = UI_CONTAINER(pObject);

    switch (pProp->id) {
        case UI_Container_Property_Id_orientation:
            GblVariant_valueCopy(pValue, &pSelf->orientation);
            break;
        case UI_Container_Property_Id_resizeWidgets:
            GblVariant_valueCopy(pValue, &pSelf->resizeWidgets);
            break;
        case UI_Container_Property_Id_alignWidgets:
            GblVariant_valueCopy(pValue, &pSelf->alignWidgets);
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
        case UI_Container_Property_Id_resizeWidgets:
            GblVariant_setBool(pValue, pSelf->resizeWidgets);
            break;
        case UI_Container_Property_Id_alignWidgets:
            GblVariant_setBool(pValue, pSelf->alignWidgets);
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

    UI_WIDGET_CLASS(pClass)->pFnActivate   = nullptr;
    UI_WIDGET_CLASS(pClass)->pFnDeactivate = nullptr;
    UI_WIDGET_CLASS(pClass)->pFnUpdate     = UI_Container_update_;

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

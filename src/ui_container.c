#include "ui_container.h"
#include "ui_widget.h"

static GBL_RESULT UI_Container_init_(GblInstance *pInstance) {
	UI_CONTAINER(pInstance)->resize_widgets = true;
	UI_CONTAINER(pInstance)->align_widgets  = true;
	UI_CONTAINER(pInstance)->padding        = 5.0f;
	UI_CONTAINER(pInstance)->margin         = 15.0f;
	return GBL_RESULT_SUCCESS;
}

static GBL_RESULT UI_Container_update_(UI_Widget *pSelf) {
	size_t child_count = GblObject_childCount(GBL_OBJECT(pSelf));
	if (child_count == 0) return GBL_RESULT_SUCCESS;

	float total_margin  = UI_CONTAINER(pSelf)->margin * (float)(child_count + 1);
	float total_padding = UI_CONTAINER(pSelf)->padding * 2.0f; // times two for top and bottom or left and right

	// VERTICAL
	if (UI_CONTAINER(pSelf)->orientation == 'v') {
		float available_h = pSelf->h - total_margin - total_padding;
		float widget_h    = available_h / (float)child_count;

		// Figure out size of children
		if (UI_CONTAINER(pSelf)->resize_widgets) {

			for (size_t i = 0; i < child_count; ++i) {
				GblObject *child_obj    = GblObject_findChildByIndex(GBL_OBJECT(pSelf), i);
				UI_Widget *child_widget = UI_WIDGET(child_obj);
				child_widget->w         = pSelf->w - 2 * UI_CONTAINER(pSelf)->padding;
				child_widget->h         = widget_h;
			}
		}

		// Figure out position of children
		if (UI_CONTAINER(pSelf)->align_widgets) {
			float y_offset = pSelf->y + UI_CONTAINER(pSelf)->padding;
			for (size_t i = 0; i < child_count; ++i) {

				// if first child, apply margin
				if (i == 0) y_offset += UI_CONTAINER(pSelf)->margin;

				GblObject *child_obj     = GblObject_findChildByIndex(GBL_OBJECT(pSelf), i);
				UI_Widget *child_widget  = UI_WIDGET(child_obj);
				child_widget->x          = pSelf->x + UI_CONTAINER(pSelf)->padding;
				child_widget->y          = y_offset;
				y_offset                += widget_h + UI_CONTAINER(pSelf)->margin;
			}
		}
	}

	// HORIZONTAL
	if (UI_CONTAINER(pSelf)->orientation == 'h') {
		float available_w = pSelf->w - total_margin - total_padding;
		float widget_w    = available_w / (float)child_count;

		// Figure out size of children
		if (UI_CONTAINER(pSelf)->resize_widgets) {
			for (size_t i = 0; i < child_count; ++i) {
				GblObject *child_obj    = GblObject_findChildByIndex(GBL_OBJECT(pSelf), i);
				UI_Widget *child_widget = UI_WIDGET(child_obj);
				child_widget->w         = widget_w;
				child_widget->h         = pSelf->h - 2 * UI_CONTAINER(pSelf)->padding;
			}
		}

		// Figure out position of children
		if (UI_CONTAINER(pSelf)->align_widgets) {
			float x_offset = pSelf->x + UI_CONTAINER(pSelf)->padding;
			for (size_t i = 0; i < child_count; ++i) {
				// if first child, apply margin
				if (i == 0) x_offset += UI_CONTAINER(pSelf)->margin;

				GblObject *child_obj     = GblObject_findChildByIndex(GBL_OBJECT(pSelf), i);
				UI_Widget *child_widget  = UI_WIDGET(child_obj);
				child_widget->x          = x_offset;
				child_widget->y          = pSelf->y + UI_CONTAINER(pSelf)->padding;
				x_offset                += widget_w + UI_CONTAINER(pSelf)->margin;
			}
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

#include "ui_widget.h"
#include "ui_button.h"

#include "ui_container.h"

static GBL_RESULT UI_Widget_init_(GblInstance *pInstance) {
	UI_WIDGET(pInstance)->x = 160;
	UI_WIDGET(pInstance)->y = 120;
	UI_WIDGET(pInstance)->w = 320;
	UI_WIDGET(pInstance)->h = 240;

	UI_WIDGET(pInstance)->is_relative = true;

	UI_WIDGET(pInstance)->r = 0;
	UI_WIDGET(pInstance)->g = 255;
	UI_WIDGET(pInstance)->b = 0;
	UI_WIDGET(pInstance)->a = 255;

	UI_WIDGET(pInstance)->border_r		= 0;
	UI_WIDGET(pInstance)->border_g		= 0;
	UI_WIDGET(pInstance)->border_b		= 0;
	UI_WIDGET(pInstance)->border_a		= 0;
	UI_WIDGET(pInstance)->border_width	= 2;

	UI_WIDGET(pInstance)->font = NULL;
	UI_WIDGET(pInstance)->font_size = 20;
	UI_WIDGET(pInstance)->font_r = 255;
	UI_WIDGET(pInstance)->font_g = 255;
	UI_WIDGET(pInstance)->font_b = 255;
	UI_WIDGET(pInstance)->font_a = 255;
	UI_WIDGET(pInstance)->font_border_r = 0;
	UI_WIDGET(pInstance)->font_border_g = 0;
	UI_WIDGET(pInstance)->font_border_b = 0;
	UI_WIDGET(pInstance)->font_border_a = 255;
	UI_WIDGET(pInstance)->font_border_thickness = 1;

	GblStringBuffer_construct(&UI_WIDGET(pInstance)->label);
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
		case UI_Widget_Property_Id_font_border_color:
			uint32_t font_border_color_;
			GblVariant_valueCopy(pValue, &font_border_color_);
			pSelf->font_border_r = (font_border_color_ >> 24) & 0xFF;
			pSelf->font_border_g = (font_border_color_ >> 16) & 0xFF;
			pSelf->font_border_b = (font_border_color_ >> 8) & 0xFF;
			pSelf->font_border_a = font_border_color_ & 0xFF;
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
		case UI_Widget_Property_Id_border_width:
			GblVariant_valueCopy(pValue, &pSelf->border_width);
			break;
		case UI_Widget_Property_Id_label:
			GblStringBuffer_set(&pSelf->label, GblVariant_toString(pValue));
			break;
		case UI_Widget_Property_Id_font_size:
			GblVariant_valueCopy(pValue, &pSelf->font_size);
			break;
		case UI_Widget_Property_Id_font_r:
			GblVariant_valueCopy(pValue, &pSelf->font_r);
			break;
		case UI_Widget_Property_Id_font_g:
			GblVariant_valueCopy(pValue, &pSelf->font_g);
			break;
		case UI_Widget_Property_Id_font_b:
			GblVariant_valueCopy(pValue, &pSelf->font_b);
			break;
		case UI_Widget_Property_Id_font_a:
			GblVariant_valueCopy(pValue, &pSelf->font_a);
			break;
		case UI_Widget_Property_Id_font_border_r:
			GblVariant_valueCopy(pValue, &pSelf->font_border_r);
			break;
		case UI_Widget_Property_Id_font_border_g:
			GblVariant_valueCopy(pValue, &pSelf->font_border_g);
			break;
		case UI_Widget_Property_Id_font_border_b:
			GblVariant_valueCopy(pValue, &pSelf->font_border_b);
			break;
		case UI_Widget_Property_Id_font_border_a:
			GblVariant_valueCopy(pValue, &pSelf->font_border_a);
			break;
		case UI_Widget_Property_Id_font_border_thickness:
			GblVariant_valueCopy(pValue, &pSelf->font_border_thickness);
			break;
		case UI_Widget_Property_Id_font:
			GblVariant_valueCopy(pValue, &pSelf->font);
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
		case UI_Widget_Property_Id_font_border_color:
			GblVariant_setUint32(pValue, pSelf->font_border_r << 24 | pSelf->font_border_g << 16 | pSelf->font_border_b << 8 | pSelf->font_border_a);
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
		case UI_Widget_Property_Id_border_width:
			GblVariant_setUint8(pValue, pSelf->border_width);
			break;
		case UI_Widget_Property_Id_label:
			GblVariant_setString(pValue, GblStringBuffer_cString(&pSelf->label));
			break;
		case UI_Widget_Property_Id_font_size:
			GblVariant_setUint8(pValue, pSelf->font_size);
			break;
		case UI_Widget_Property_Id_font_r:
			GblVariant_setUint8(pValue, pSelf->font_r);
			break;
		case UI_Widget_Property_Id_font_g:
			GblVariant_setUint8(pValue, pSelf->font_g);
			break;
		case UI_Widget_Property_Id_font_b:
			GblVariant_setUint8(pValue, pSelf->font_b);
			break;
		case UI_Widget_Property_Id_font_a:
			GblVariant_setUint8(pValue, pSelf->font_a);
			break;
		case UI_Widget_Property_Id_font_border_r:
			GblVariant_setUint8(pValue, pSelf->font_border_r);
			break;
		case UI_Widget_Property_Id_font_border_g:
			GblVariant_setUint8(pValue, pSelf->font_border_g);
			break;
		case UI_Widget_Property_Id_font_border_b:
			GblVariant_setUint8(pValue, pSelf->font_border_b);
			break;
		case UI_Widget_Property_Id_font_border_a:
			GblVariant_setUint8(pValue, pSelf->font_border_a);
			break;
		case UI_Widget_Property_Id_font_border_thickness:
			GblVariant_setUint8(pValue, pSelf->font_border_thickness);
			break;
		case UI_Widget_Property_Id_font:
			GblVariant_setPointer(pValue, UI_FONT_TYPE, &pSelf->font);
			break;
		default:
			return GBL_RESULT_ERROR_INVALID_PROPERTY;
	}

	return GBL_RESULT_SUCCESS;
}

Vector2 UI_get_absolute_position(UI_Widget *pWidget) {
	GblObject	*parent	= GblObject_parent(GBL_OBJECT(pWidget));
	Vector2		pos		= { pWidget->x, pWidget->y };

	if (!parent || !pWidget->is_relative) {
		return pos;
	}

	Vector2 parent_pos = UI_get_absolute_position(UI_WIDGET(parent));

	pos.x += parent_pos.x;
	pos.y += parent_pos.y;

	return pos;
}

static GBL_RESULT UI_Widget_update_(UI_Widget *pSelf) {
	//
}

static GBL_RESULT UI_Widget_draw_(UI_Widget *pSelf) {
	Rectangle  rec    = (Rectangle){ pSelf->x, pSelf->y, pSelf->w, pSelf->h };
	GblObject *parent = GblObject_parent(GBL_OBJECT(pSelf));

	if (parent && GBL_TYPEOF(parent) != UI_CONTAINER_TYPE && pSelf->is_relative) {
		Vector2 parent_pos	= UI_get_absolute_position(UI_WIDGET(parent));
		rec.x				+= parent_pos.x;
		rec.y				+= parent_pos.y;
	}

	if (pSelf->a) {
		DrawRectangleRec(rec, (Color){ pSelf->r, pSelf->g, pSelf->b, pSelf->a });
	}

	if (pSelf->border_a) {
		DrawRectangleLinesEx(rec, pSelf->border_width, (Color){ pSelf->border_r, pSelf->border_g, pSelf->border_b, pSelf->border_a });
	}

	if (GblStringBuffer_length(&pSelf->label)) {
		Font font;
		if (pSelf->font == NULL) {
			font = GetFontDefault();
		} else {
			font = *(pSelf->font);
		}

		Vector2 text_size = MeasureTextEx(font, GblStringBuffer_cString(&pSelf->label), pSelf->font_size, 1);
		float text_width = text_size.x;

		// border
		if (pSelf->font_border_a && pSelf->font_border_thickness) {
			for (int dx = -pSelf->font_border_thickness; dx <= pSelf->font_border_thickness; dx++) {
				for (int dy = -pSelf->font_border_thickness; dy <= pSelf->font_border_thickness; dy++) {
					if (dx == 0 && dy == 0) continue;

					DrawTextEx(font,
						GblStringBuffer_cString(&pSelf->label),
						(Vector2){ rec.x + (rec.width - text_width) / 2 + dx, rec.y + (rec.height - pSelf->font_size) / 2 + dy },
						pSelf->font_size,
						1,
						(Color){ pSelf->font_border_r, pSelf->font_border_g, pSelf->font_border_b, pSelf->font_border_a });
				}
			}
		}

		DrawTextEx(font,
			GblStringBuffer_cString(&pSelf->label),
			(Vector2){ rec.x + (rec.width - text_width) / 2, rec.y + (rec.height - pSelf->font_size) / 2 },
			(float)pSelf->font_size,
			1,
			(Color){ pSelf->font_r, pSelf->font_g, pSelf->font_b, pSelf->font_a });
	}

}

static GBL_RESULT UI_WidgetClass_init_(GblClass *pClass, const void *pData) {
	GBL_UNUSED(pData);

	// Check if this is the first instance of the class
	if (!GblType_classRefCount(GBL_CLASS_TYPEOF(pClass))) GBL_PROPERTIES_REGISTER(UI_Widget);

	GBL_OBJECT_CLASS(pClass)->pFnSetProperty = UI_Widget_GblObject_setProperty_;
	GBL_OBJECT_CLASS(pClass)->pFnProperty    = UI_Widget_GblObject_property_;

	UI_WIDGET_CLASS(pClass)->pFnActivate	= NULL;
	UI_WIDGET_CLASS(pClass)->pFnDeactivate	= NULL;
	UI_WIDGET_CLASS(pClass)->pFnUpdate		= UI_Widget_update_;
	UI_WIDGET_CLASS(pClass)->pFnDraw		= UI_Widget_draw_;

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

void UI_make_child_(UI_Widget *pParent, UI_Widget *pChild) {
	GblObject_setParent(GBL_OBJECT(pChild), GBL_OBJECT(pParent));
}

GBL_RESULT UI_update_(UI_Widget *pSelf) {
	UI_WIDGET_GET_CLASS(pSelf)->pFnUpdate(pSelf);

	size_t child_count = GblObject_childCount(GBL_OBJECT(pSelf));

	for (size_t i = 0; i < child_count; ++i) {
		GblObject *child_obj = GblObject_findChildByIndex(GBL_OBJECT(pSelf), i);
		UI_update_(UI_WIDGET(child_obj));
	}

	return GBL_RESULT_SUCCESS;
}

GBL_RESULT UI_draw_(UI_Widget *pSelf) {
	UI_WIDGET_GET_CLASS(pSelf)->pFnDraw(pSelf);

	size_t child_count = GblObject_childCount(GBL_OBJECT(pSelf));

	for (size_t i = 0; i < child_count; ++i) {
		GblObject *child_obj = GblObject_findChildByIndex(GBL_OBJECT(pSelf), i);
		UI_draw_(UI_WIDGET(child_obj));
	}

	return GBL_RESULT_SUCCESS;
}

UI_Widget *UI_Widget_ref(UI_Widget *pSelf) {
	return UI_WIDGET(GBL_REF(pSelf));
}

void UI_unref_(UI_Widget *pSelf) {
	const char *name	= GblType_name(GBL_TYPEOF(pSelf));
	size_t child_count	= GblObject_childCount(GBL_OBJECT(pSelf));

	for (size_t i = 0; i < child_count; ++i) {
		GblObject *child_obj = GblObject_findChildByIndex(GBL_OBJECT(pSelf), i);
		UI_unref_(UI_WIDGET(child_obj));
	}

	GBL_UNREF(pSelf);
}

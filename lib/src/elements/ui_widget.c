#include <ui/elements/ui_widget.h>
#include <ui/elements/ui_button.h>
#include <ui/elements/ui_container.h>
#include <ui/elements/ui_root.h>
#include <ui/ui_general.h>

static GBL_RESULT UI_Widget_init_(GblInstance *pInstance) {
	UI_Widget *pSelf = UI_WIDGET(pInstance);

	pSelf->z_index = 0;

	pSelf->x 		  = 160;
	pSelf->y 		  = 120;
	pSelf->w 		  = 320;
	pSelf->h 		  = 240;
	pSelf->isRelative = false;

	pSelf->r = 0;
	pSelf->g = 255;
	pSelf->b = 0;
	pSelf->a = 255;

	pSelf->border_r			= 0;
	pSelf->border_g			= 0;
	pSelf->border_b			= 0;
	pSelf->border_a			= 0;
	pSelf->border_width		= 4;
	pSelf->border_radius	= 0.0f;
	pSelf->border_highlight = false;

	pSelf->font 				 = nullptr;
	pSelf->font_size 			 = 22;
	pSelf->font_r 				 = 255;
	pSelf->font_g 				 = 255;
	pSelf->font_b 				 = 255;
	pSelf->font_a 		 		 = 255;
	pSelf->font_border_r 		 = 0;
	pSelf->font_border_g 		 = 0;
	pSelf->font_border_b 		 = 0;
	pSelf->font_border_a 		 = 0;
	pSelf->font_border_thickness = 1;
	pSelf->textAlignment 	  	 = UI_TEXT_ALIGN_CENTER;

	pSelf->texture = nullptr;

	auto root = GBL_REQUIRE(UI_Root, "UI_Root");
	if GBL_UNLIKELY(!root) return GBL_RESULT_ERROR;
	GblObject_setParent(GBL_OBJECT(pInstance), GBL_OBJECT(root));

	UI_drawQueue_push(GBL_OBJECT(pInstance));

	return GBL_RESULT_SUCCESS;
}

static GBL_RESULT UI_Widget_deactivate_(UI_Widget *pSelf) {
	GblStringRef_unref(pSelf->label);
	return GBL_RESULT_SUCCESS;
}

static GBL_RESULT UI_Widget_GblObject_setProperty_(GblObject *pObject, const GblProperty *pProp, GblVariant *pValue) {
	UI_Widget *pSelf = UI_WIDGET(pObject);
	switch (pProp->id) {
		case UI_Widget_Property_Id_z_index:
			GblVariant_valueCopy(pValue, &pSelf->z_index);
			break;
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
		case UI_Widget_Property_Id_isRelative:
			GblVariant_valueCopy(pValue, &pSelf->isRelative);
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
		case UI_Widget_Property_Id_border_radius:
			GblVariant_valueCopy(pValue, &pSelf->border_radius);
			break;
		case UI_Widget_Property_Id_border_highlight:
			GblVariant_valueCopy(pValue, &pSelf->border_highlight);
			break;
		case UI_Widget_Property_Id_label:
			GblStringRef_unref(pSelf->label);
			pSelf->label = GblStringRef_create(GblVariant_string(pValue));
			break;
		case UI_Widget_Property_Id_textAlignment:
			GblVariant_valueCopy(pValue, &pSelf->textAlignment);
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
		case UI_Widget_Property_Id_texture:
			GblVariant_valueCopy(pValue, &pSelf->texture);
			break;
		case UI_Widget_Property_Id_parent:
			GblObject *pOldParent = GblObject_parent(pObject);
			if(pOldParent) {
				GblObject_removeChild(pOldParent, pObject);
			}

			GblObject *pNewParent = GblVariant_objectPeek(pValue);
			GblObject_addChild(pNewParent, pObject);
			break;
		default:
			return GBL_RESULT_ERROR_INVALID_PROPERTY;
	}

	return GBL_RESULT_SUCCESS;
}

static GBL_RESULT UI_Widget_GblObject_property_(const GblObject *pObject, const GblProperty *pProp, GblVariant *pValue) {
	UI_Widget *pSelf = UI_WIDGET(pObject);

	switch (pProp->id) {
		case UI_Widget_Property_Id_z_index:
			GblVariant_setUint8(pValue, pSelf->z_index);
			break;
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
		case UI_Widget_Property_Id_isRelative:
			GblVariant_setBool(pValue, pSelf->isRelative);
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
		case UI_Widget_Property_Id_border_radius:
			GblVariant_setFloat(pValue, pSelf->border_radius);
			break;
		case UI_Widget_Property_Id_border_highlight:
			GblVariant_setUint8(pValue, pSelf->border_highlight);
			break;
		case UI_Widget_Property_Id_label:
			GblVariant_setString(pValue, pSelf->label);
			break;
		case UI_Widget_Property_Id_textAlignment:
			GblVariant_setEnum(pValue, UI_TEXT_ALIGNMENT_TYPE, pSelf->textAlignment);
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
		case UI_Widget_Property_Id_texture:
			GblVariant_setPointer(pValue, UI_TEXTURE_TYPE, &pSelf->texture);
			break;
		case UI_Widget_Property_Id_parent:
			GblVariant_setPointer(pValue, GBL_OBJECT_TYPE, GblObject_parent(GBL_OBJECT(pSelf)));
			break;
		default:
			return GBL_RESULT_ERROR_INVALID_PROPERTY;
	}

	return GBL_RESULT_SUCCESS;
}

static GBL_RESULT UI_Widget_update_(UI_Widget *pSelf) {
	return GBL_RESULT_SUCCESS;
}

static GBL_RESULT UI_Widget_draw_(UI_Widget *pSelf) {
	Rectangle  rec    = (Rectangle){ pSelf->x, pSelf->y, pSelf->w, pSelf->h };
	UI_Widget *pParent = GBL_AS(UI_Widget, GblObject_parent(GBL_OBJECT(pSelf)));

	if (pParent && pSelf->isRelative) {
		Vector2 parent_pos	= UI_get_absolute_position_(UI_WIDGET(pParent));
		rec.x				+= parent_pos.x;
		rec.y				+= parent_pos.y;
	}

	if (pSelf->a) {
		DrawRectangleRounded(rec, 0.15f, 6, (Color){ pSelf->r, pSelf->g, pSelf->b, pSelf->a });
	}

	UI_Button *pButton = GBL_AS(UI_Button, pSelf);

	bool isSelected = pButton && pButton->isSelected;

	if (pSelf->border_a && !isSelected) {
		DrawRectangleRoundedLinesEx(rec, pSelf->border_radius, 6, pSelf->border_width, (Color){ pSelf->border_r, pSelf->border_g, pSelf->border_b, pSelf->border_a });

		if (pSelf->border_highlight) {
			float inner_thickness = 1;
			float inset = (pSelf->border_width - inner_thickness) / 2.0f;
			Rectangle inner = {
				rec.x - inset / 2,
				rec.y - inset / 2,
				rec.width  + inset,
				rec.height + inset
			};
			DrawRectangleRoundedLinesEx(inner, pSelf->border_radius, 6, inner_thickness, (Color){ 255, 255, 255, 255 });
		}
	}

	// text and texture rendering
	Vector2 textSize	= { 0, 0 };
	Vector2 textPos		= { 0, 0 };
	const float margin	= 3.0f;

	if (GblStringRef_length(pSelf->label)) {
		Font font;
		if (!pSelf->font) {
			font = GetFontDefault();
		} else {
			font = *(pSelf->font);
		}

		textSize = MeasureTextEx(font, pSelf->label, pSelf->font_size, 1);

		switch (pSelf->textAlignment) {
			case UI_TEXT_ALIGN_CENTER:
				textPos = (Vector2){ rec.x + (rec.width - textSize.x) / 2, rec.y + rec.height / 2 - textSize.y / 2 };

				// can't align to the center if there's a texture, so default to bottom
				if (pSelf->texture) {
					textPos = (Vector2){ rec.x + (rec.width - textSize.x) / 2, rec.y + rec.height - textSize.y - margin };
				}

				break;
			case UI_TEXT_ALIGN_TOP:
				textPos = (Vector2){ rec.x + (rec.width - textSize.x) / 2, rec.y + textSize.y / 2 + margin };
				break;
			case UI_TEXT_ALIGN_RIGHT:
				textPos = (Vector2){ rec.x + rec.width - textSize.x - margin, rec.y + (rec.height - textSize.y) / 2 };
				break;
			case UI_TEXT_ALIGN_BOTTOM:
				textPos = (Vector2){ rec.x + (rec.width - textSize.x) / 2, rec.y + rec.height - textSize.y - margin };
				break;
			case UI_TEXT_ALIGN_LEFT:
				textPos = (Vector2){ rec.x + margin, rec.y + (rec.height - pSelf->font_size) / 2 };
				break;
		}


		// text border
		if (pSelf->font_border_a && pSelf->font_border_thickness) {
			for (int dx = -pSelf->font_border_thickness; dx <= pSelf->font_border_thickness; dx++) {
				for (int dy = -pSelf->font_border_thickness; dy <= pSelf->font_border_thickness; dy++) {
					if (dx == 0 && dy == 0) continue;

					DrawTextEx(font,
							   pSelf->label,
							   (Vector2){ textPos.x + dx, textPos.y + dy },
							   pSelf->font_size,
							   1.2,
							   (Color){ pSelf->font_border_r, pSelf->font_border_g, pSelf->font_border_b, pSelf->font_border_a });
				}
			}
		}

		DrawTextEx(font,
				   pSelf->label,
				   textPos,
				   (float)pSelf->font_size,
				   1.2,
				   (Color){ pSelf->font_r, pSelf->font_g, pSelf->font_b, pSelf->font_a });
	}


	if (pSelf->texture) {
		Vector2 textureSize		= { (float)pSelf->texture->width, (float)pSelf->texture->height };
		Vector2 texturePos		= { rec.x, rec.y };

		// adjust texture size based on pSelf size
		textureSize.x = rec.width;
		textureSize.y = rec.height;

		// if there is text, shrink it
		if (GblStringRef_length(pSelf->label)) {
			textureSize.y *= 0.6f;
			textureSize.x *= 0.6f;
		}

		// adjust texture position based on text size, position and alignment
		switch (pSelf->textAlignment) {
			case UI_TEXT_ALIGN_CENTER:
			case UI_TEXT_ALIGN_BOTTOM:
				texturePos = (Vector2){ rec.x + (rec.width - textureSize.x) / 2, rec.y + (rec.height - textureSize.y) / 2 - margin - textSize.y / 2 };
				break;
			case UI_TEXT_ALIGN_TOP:
				texturePos = (Vector2){ rec.x + (rec.width - textureSize.x) / 2, rec.y + (rec.height - textureSize.y) / 2 + margin + textSize.y / 2 };
				break;
			case UI_TEXT_ALIGN_LEFT:
				texturePos = (Vector2){ rec.x + (rec.width - textureSize.x) / 2 + margin + textSize.x / 2, rec.y + (rec.height - textureSize.y) / 2 };
				break;
			case UI_TEXT_ALIGN_RIGHT:
				texturePos = (Vector2){ rec.x + (rec.width - textureSize.x) / 2 - margin - textSize.x / 2, rec.y + (rec.height - textureSize.y) / 2 };
				break;
		}


		Rectangle src = { 0, 0, (float)pSelf->texture->width, (float)pSelf->texture->height };
		Rectangle dst = { texturePos.x, texturePos.y, textureSize.x, textureSize.y };

		DrawTexturePro(*pSelf->texture, src, dst, (Vector2){ 0, 0 }, 0.0f, (Color){ 255, 255, 255, 255 });
	}

	return GBL_RESULT_SUCCESS;
}

static GBL_RESULT UI_WidgetClass_init_(GblClass *pClass, const void *pData) {
	GBL_UNUSED(pData);

	if (!GblType_classRefCount(UI_WIDGET_TYPE)) GBL_PROPERTIES_REGISTER(UI_Widget);

	GBL_OBJECT_CLASS(pClass)->pFnSetProperty = UI_Widget_GblObject_setProperty_;
	GBL_OBJECT_CLASS(pClass)->pFnProperty    = UI_Widget_GblObject_property_;
	UI_WIDGET_CLASS(pClass)->pFnActivate	 = nullptr;
	UI_WIDGET_CLASS(pClass)->pFnDeactivate	 = UI_Widget_deactivate_;
	UI_WIDGET_CLASS(pClass)->pFnUpdate		 = UI_Widget_update_;
	UI_WIDGET_CLASS(pClass)->pFnDraw		 = UI_Widget_draw_;

	return GBL_RESULT_SUCCESS;
}

GblType UI_Widget_type(void) {
	static GblType type = GBL_INVALID_TYPE;

	if (type == GBL_INVALID_TYPE) {
		type =
			GblType_register(GblQuark_internStatic("UI_Widget"),
							 GBL_OBJECT_TYPE,
							 &(static GblTypeInfo){.classSize = sizeof(UI_WidgetClass),
												   .pFnClassInit = UI_WidgetClass_init_,
												   .instanceSize = sizeof(UI_Widget),
												   .pFnInstanceInit = UI_Widget_init_},
							 GBL_TYPE_FLAG_TYPEINFO_STATIC);
	}

	return type;
}

Vector2 UI_get_absolute_position_(UI_Widget *pWidget) {
	UI_Widget	*parent	= GBL_AS(UI_Widget, GblObject_parent(GBL_OBJECT(pWidget)));
	Vector2		pos		= { pWidget->x, pWidget->y };

	if (!parent || !pWidget->isRelative) {
		return pos;
	}

	if (GBL_AS(UI_Container, parent)) {
		if (!GBL_AS(UI_Container, parent)->alignWidgets) return pos;
	}

	Vector2 parent_pos = UI_get_absolute_position_(UI_WIDGET(parent));

	pos.x += parent_pos.x;
	pos.y += parent_pos.y;

	return pos;
}
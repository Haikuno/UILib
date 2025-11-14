#ifndef GUM_WIDGET_H
#define GUM_WIDGET_H

#include <gimbal/gimbal_meta.h>
#include <gimbal/gimbal_strings.h>
#include <gimbal/gimbal_core.h>
#include <raylib.h>

#include <gumball/types/gumball_font.h>
#include <gumball/types/gumball_texture.h>
#include <gumball/types/gumball_renderer.h>

#define GUM_WIDGET_TYPE				(GBL_TYPEID		(GUM_Widget))
#define GUM_WIDGET(self)			(GBL_CAST		(GUM_Widget, self))
#define GUM_WIDGET_CLASS(klass)		(GBL_CLASS_CAST (GUM_Widget, klass))
#define GUM_WIDGET_CLASSOF(self)	(GBL_CLASSOF	(GUM_Widget, self))

#define GBL_SELF_TYPE GUM_Widget

GBL_FORWARD_DECLARE_STRUCT(GUM_Widget);
GBL_DECLS_BEGIN

GBL_CLASS_DERIVE(GUM_Widget, GblObject)
	GBL_RESULT (*pFnActivate)(GBL_SELF);
	GBL_RESULT (*pFnDeactivate)(GBL_SELF);
	GBL_RESULT (*pFnUpdate)(GBL_SELF);
	GBL_RESULT (*pFnDraw)(GBL_SELF, GUM_Renderer *pRenderer);
GBL_CLASS_END

GBL_INSTANCE_DERIVE(GUM_Widget, GblObject)
	float   			x, y, w, h;																// if the widget is a child of another widget, then x & y are relative
	uint8_t 			r, g, b, a;																// background color values
	uint8_t 			border_r, border_g, border_b, border_a, border_width;				   	// border color values and border width (set border_a to 0 for no border)
	float 				border_radius;															// border radius for rounded corners
	bool				border_highlight;														// if the border should be highlighted (a white, smaller line running through the border)
	bool				isRelative;																// if the position of this widget should be relative to its parent (if it has one)
	GblStringRef 		*label;																	// optional text label for the widget
	GUM_Font			*font;																	// font for rendering text
	GUM_TextAlignment	textAlignment;															// text alignment // GUM_TODO: make this work lol
	GUM_Texture			*texture;																// optional texture for rendering
	uint8_t  			font_size;																// font size for rendering text
	uint8_t				font_r, font_g, font_b, font_a;                         				// font color values
	uint8_t				font_border_r, font_border_g, font_border_b, font_border_a; 			// font border color values (set border_a to 0 for no border)
	uint8_t				font_border_thickness;													// font border thickness
	uint8_t				z_index;																// z-index for rendering order
GBL_INSTANCE_END

GBL_PROPERTIES(GUM_Widget,
	(x,						GBL_GENERIC, (READ, WRITE), GBL_FLOAT_TYPE),
	(y,						GBL_GENERIC, (READ, WRITE), GBL_FLOAT_TYPE),
	(w,						GBL_GENERIC, (READ, WRITE), GBL_FLOAT_TYPE),
	(h,						GBL_GENERIC, (READ, WRITE), GBL_FLOAT_TYPE),
	(isRelative,			GBL_GENERIC, (READ, WRITE), GBL_BOOL_TYPE),
	(color,					GBL_GENERIC, (READ, WRITE), GBL_UINT32_TYPE),
	(border_color,			GBL_GENERIC, (READ, WRITE), GBL_UINT32_TYPE),
	(font_border_color,		GBL_GENERIC, (READ, WRITE), GBL_UINT32_TYPE),
	(r,						GBL_GENERIC, (READ, WRITE), GBL_UINT8_TYPE),
	(g,						GBL_GENERIC, (READ, WRITE), GBL_UINT8_TYPE),
	(b,						GBL_GENERIC, (READ, WRITE), GBL_UINT8_TYPE),
	(a,						GBL_GENERIC, (READ, WRITE), GBL_UINT8_TYPE),
	(border_r,				GBL_GENERIC, (READ, WRITE), GBL_UINT8_TYPE),
	(border_g,				GBL_GENERIC, (READ, WRITE), GBL_UINT8_TYPE),
	(border_b,				GBL_GENERIC, (READ, WRITE), GBL_UINT8_TYPE),
	(border_a,				GBL_GENERIC, (READ, WRITE), GBL_UINT8_TYPE),
	(border_width,			GBL_GENERIC, (READ, WRITE), GBL_UINT8_TYPE),
	(border_radius,			GBL_GENERIC, (READ, WRITE), GBL_FLOAT_TYPE),
	(border_highlight,		GBL_GENERIC, (READ, WRITE), GBL_BOOL_TYPE),
	(label,					GBL_GENERIC, (READ, WRITE), GBL_STRING_TYPE),
	(texture,				GBL_GENERIC, (READ, WRITE), GUM_IRESOURCE_TYPE),
	(textAlignment,			GBL_GENERIC, (READ, WRITE), GUM_TEXT_ALIGNMENT_TYPE),
	(font,					GBL_GENERIC, (READ, WRITE), GUM_FONT_TYPE),
	(font_size,				GBL_GENERIC, (READ, WRITE), GBL_UINT8_TYPE),
	(font_r,				GBL_GENERIC, (READ, WRITE), GBL_UINT8_TYPE),
	(font_g,				GBL_GENERIC, (READ, WRITE), GBL_UINT8_TYPE),
	(font_b,				GBL_GENERIC, (READ, WRITE), GBL_UINT8_TYPE),
	(font_a,				GBL_GENERIC, (READ, WRITE), GBL_UINT8_TYPE),
	(font_border_r,			GBL_GENERIC, (READ, WRITE), GBL_UINT8_TYPE),
	(font_border_g,			GBL_GENERIC, (READ, WRITE), GBL_UINT8_TYPE),
	(font_border_b,			GBL_GENERIC, (READ, WRITE), GBL_UINT8_TYPE),
	(font_border_a,			GBL_GENERIC, (READ, WRITE), GBL_UINT8_TYPE),
	(font_border_thickness, GBL_GENERIC, (READ, WRITE), GBL_UINT8_TYPE),
	(z_index,				GBL_GENERIC, (READ, WRITE), GBL_UINT8_TYPE),
	(parent,				GBL_GENERIC, (READ, WRITE), GBL_OBJECT_TYPE)
)

GblType GUM_Widget_type(void);

Vector2	GUM_get_absolute_position_(GBL_SELF);

// Takes a list of Name/Value pairs
#define GUM_Widget_create(/* property_name, property_value */...) GBL_NEW(GUM_Widget __VA_OPT__(,) __VA_ARGS__)

GBL_DECLS_END
#undef GBL_SELF_TYPE

#endif // GUM_WIDGET_H

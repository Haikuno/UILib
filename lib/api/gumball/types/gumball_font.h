#ifndef GUM_FONT_H
#define GUM_FONT_H

#include <gimbal/gimbal_meta.h>
#include <gumball/types/gumball_vector2.h>

#define GUM_FONT_TYPE (GBL_TYPEID(GUM_Font))
#define GUM_TEXT_ALIGNMENT_TYPE (GBL_TYPEID(GUM_TextAlignment))

GBL_FORWARD_DECLARE_STRUCT(GUM_Font);
GBL_DECLS_BEGIN

typedef enum GUM_TextAlignment {
	GUM_TEXT_ALIGN_CENTER,
	GUM_TEXT_ALIGN_TOP,
	GUM_TEXT_ALIGN_RIGHT,
	GUM_TEXT_ALIGN_BOTTOM,
	GUM_TEXT_ALIGN_LEFT
} GUM_TextAlignment;

GblType GUM_Font_type(void);
GblType	GUM_TextAlignment_type(void);

GUM_Font 	  *GUM_Font_create(void *pFont);
GUM_Vector2    GUM_Font_measureText(GUM_Font *pFont, GblStringRef *pText, uint8_t fontSize);

GBL_DECLS_END

#endif // GUM_FONT_H

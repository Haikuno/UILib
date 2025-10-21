#ifndef UI_FONT_H
#define UI_FONT_H

#include <gimbal/gimbal_meta.h>

#define UI_FONT_TYPE (GBL_TYPEID(UI_Font))
#define UI_TEXT_ALIGNMENT_TYPE (GBL_TYPEID(UI_TextAlignment))

typedef enum UI_TextAlignment {
	UI_TEXT_ALIGN_CENTER,
	UI_TEXT_ALIGN_TOP,
	UI_TEXT_ALIGN_RIGHT,
	UI_TEXT_ALIGN_BOTTOM,
	UI_TEXT_ALIGN_LEFT
} UI_TextAlignment;

GblType UI_Font_type(void);
GblType	UI_TextAlignment_type(void);

#endif // UI_FONT_H

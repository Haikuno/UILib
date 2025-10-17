#include "ui_font.h"

GblType UI_Font_type(void) {
    static GblType type = GBL_INVALID_TYPE;

    if (type == GBL_INVALID_TYPE) {
	type = GblPointer_register("UI_Font");
    }

    return type;
}
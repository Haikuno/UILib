#include <ui/types/ui_font.h>

GblType UI_Font_type(void) {
    static GblType type = GBL_INVALID_TYPE;

    if (type == GBL_INVALID_TYPE) {
        type = GblPointer_register("UI_Font");
    }

    return type;
}

GblType UI_TextAlignment_type(void) {
    static GblType type = GBL_INVALID_TYPE;

    if (type == GBL_INVALID_TYPE) {
        const static GblEnumEntry enumEntries[] = {
            GBL_ENUM_ENTRY(UI_TEXT_ALIGN_TOP,    "Top"   ),
            GBL_ENUM_ENTRY(UI_TEXT_ALIGN_RIGHT,  "Right" ),
            GBL_ENUM_ENTRY(UI_TEXT_ALIGN_BOTTOM, "Bottom"),
            GBL_ENUM_ENTRY(UI_TEXT_ALIGN_LEFT,   "Left"  ),
            GBL_ENUM_ENTRY(UI_TEXT_ALIGN_CENTER, "Center")
        };

        type = GblEnum_register("TextAlignment", enumEntries);
    }

    return type;
}
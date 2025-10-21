#include "ui_texture.h"

GblType UI_Texture_type(void) {
    static GblType type = GBL_INVALID_TYPE;

    if (type == GBL_INVALID_TYPE) {
        type = GblPointer_register("UI_Texture");
    }

    return type;
}

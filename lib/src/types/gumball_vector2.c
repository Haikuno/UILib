#include <gumball/types/gumball_vector2.h>

GblType GUM_Vector2_type(void) {
    static GblType type = GBL_INVALID_TYPE;

    if (type == GBL_INVALID_TYPE) {
        type = GblPointer_register("GUM_vector2");
    }

    return type;
}

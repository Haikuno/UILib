#ifndef GUM_VECTOR2_H
#define GUM_VECTOR2_H

#include <gimbal/gimbal_meta.h>

GBL_DECLS_BEGIN

typedef struct {
	float x;
	float y;
} GUM_Vector2;

#define GUM_VECTOR2_TYPE (GBL_TYPEID(GUM_Vector2))

GblType GUM_Vector2_type(void);

GBL_DECLS_END

#endif // GUM_VECTOR2_H

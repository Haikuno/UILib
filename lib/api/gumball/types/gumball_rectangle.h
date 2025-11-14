#ifndef GUM_RECTANGLE_H
#define GUM_RECTANGLE_H

#include <gimbal/gimbal_meta.h>

GBL_DECLS_BEGIN

typedef struct {
	float x;
	float y;
	float width;
	float height;
} GUM_Rectangle;

#define GUM_RECTANGLE_TYPE (GBL_TYPEID(GUM_Rectangle))

GblType GUM_Rectangle_type(void);

GBL_DECLS_END

#endif // GUM_RECTANGLE_H

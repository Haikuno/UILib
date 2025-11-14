#ifndef GUM_COLOR_H
#define GUM_COLOR_H

#include <gimbal/gimbal_meta.h>

GBL_DECLS_BEGIN

typedef struct {
	float r;
	float g;
	float b;
	float a;
} GUM_Color;

#define GUM_COLOR_TYPE (GBL_TYPEID(GUM_Color))

GblType GUM_Color_type(void);

GBL_DECLS_END

#endif // GUM_COLOR_H

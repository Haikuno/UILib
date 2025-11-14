#ifndef GUM_RENDERER_H
#define GUM_RENDERER_H

#include <gimbal/gimbal_meta.h>


typedef struct GUM_Renderer GUM_Renderer;
GBL_DECLS_BEGIN

GUM_Renderer *GUM_Renderer_create(void *pRenderer);

#define GUM_RENDERER_TYPE (GBL_TYPEID(GUM_Renderer))

GblType GUM_Renderer_type(void);

GBL_DECLS_END

#endif // GUM_Renderer_H

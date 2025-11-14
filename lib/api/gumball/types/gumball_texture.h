#ifndef GUM_TEXTURE_H
#define GUM_TEXTURE_H

#include <gimbal/gimbal_meta.h>
#include <gumball/types/gumball_vector2.h>
#include <gumball/ifaces/gumball_iresource.h>

#define GUM_TEXTURE_TYPE 		  (GBL_TYPEID	  (GUM_Texture))
#define GUM_TEXTURE(self) 		  (GBL_CAST		  (GUM_Texture, self))
#define GUM_TEXTURE_CLASS(klass)  (GBL_CLASS_CAST (GUM_Texture, klass))
#define GUM_TEXTURE_CLASSOF(self) (GBL_CLASSOF	  (GUM_Texture, self))

#define GBL_SELF_TYPE GUM_Texture

GBL_DECLS_BEGIN

GBL_FORWARD_DECLARE_STRUCT(GUM_Texture);

GBL_CLASS_DERIVE_EMPTY(GUM_Texture, GblBox, GUM_IResource)

GBL_INSTANCE_DERIVE_EMPTY(GUM_Texture, GblBox)

GblType GUM_Texture_type(void);

GUM_Vector2 GUM_Texture_size(GBL_SELF);

void GUM_Texture_loadFromBytes(GBL_SELF);

// GUM_Texture *GUM_Texture_create(GBL_SELF);

GBL_DECLS_END

#undef GBL_SELF_TYPE

#endif // GUM_TEXTURE_H

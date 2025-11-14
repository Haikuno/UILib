#ifndef GUM_IRESOURCE_H
#define GUM_IRESOURCE_H

#include <gimbal/gimbal_meta.h>
#include <gimbal/utils/gimbal_byte_array.h>

#define GUM_IRESOURCE_TYPE				(GBL_TYPEID		(GUM_IResource))
#define GUM_IRESOURCE(instance)	        (GBL_CAST		(GUM_IResource, instance))
#define GUM_IRESOURCE_CLASS(klass)		(GBL_CLASS_CAST (GUM_IResource, klass))
#define GUM_IRESOURCE_CLASSOF(instance)	(GBL_CLASSOF	(GUM_IResource, instance))

#define GBL_SELF_TYPE 					 GUM_IResource

GBL_DECLS_BEGIN

GBL_FORWARD_DECLARE_STRUCT(GUM_IResource);

GBL_INTERFACE_DERIVE(GUM_IResource)
	GUM_IResource*  (*pFnRef)		(GBL_SELF);
	GblRefCount 	(*pFnUnref)		(GBL_SELF);
	GBL_RESULT 		(*pFnCreate)	(GBL_SELF, GblByteArray **ppByteArray, GblQuark quark, GblStringRef *extension);
GBL_INTERFACE_END

GblType GUM_IResource_type(void);

GUM_IResource *GUM_IResource_ref(GUM_IResource *pResource);
GblRefCount   GUM_IResource_unref(GUM_IResource *pResource);

GBL_DECLS_END

#undef GBL_SELF_TYPE

#endif // GUM_IRESOURCE_H

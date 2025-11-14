#ifndef GUM_MANAGER_H
#define GUM_MANAGER_H

#include <gimbal/gimbal_meta.h>
#include <gumball/ifaces/gumball_iresource.h>

#define GUM_MANAGER_TYPE				(GBL_TYPEID(GUM_Manager))
#define GUM_MANAGER(self)				(GBL_CAST(GUM_Manager, self))
#define GUM_MANAGER_CLASS(klass)		(GBL_CLASS_CAST(GUM_Manager, klass))
#define GUM_MANAGER_CLASSOF(self)		(GBL_CLASSOF(GUM_Manager, self))

#define GBL_SELF_TYPE GUM_Manager

GBL_FORWARD_DECLARE_STRUCT(GUM_Manager);
GBL_DECLS_BEGIN

GBL_STATIC_CLASS_DERIVE_EMPTY(GUM_Manager)

GblType GUM_Manager_type(void);

// Loads a resource from the given path. If the resource has already been loaded, returns a reference to the existing resource.
GUM_IResource *GUM_Manager_load(GblStringRef *path);

// Unloads a resource from the manager, removing it from the internal hashset.
void GUM_Manager_unload(GUM_IResource *pResource);

GBL_DECLS_END
#undef GBL_SELF_TYPE

#endif // GUM_MANAGER_H
#include <gumball/ifaces/gumball_iresource.h>

GUM_IResource *GUM_IResource_ref(GUM_IResource* pResource) {
	return GUM_IRESOURCE(GBL_REF(pResource));
}

GblRefCount GUM_IResource_unref(GUM_IResource* pResource) {
	return GBL_UNREF(pResource);
}

GblType GUM_IResource_type(void) {
	static GblType type = GBL_INVALID_TYPE;
    static GblType dependencies[] = {GBL_INVALID_TYPE};

	if GBL_UNLIKELY(type == GBL_INVALID_TYPE) {
        dependencies[0] = GBL_BOX_TYPE;
		type = GblType_register(GblQuark_internStatic("GUM_IResource"),
							 	GBL_INTERFACE_TYPE,
							 	&(static GblTypeInfo){.classSize     = sizeof(GUM_IResourceClass),
											          .pDependencies = dependencies,
													  .dependencyCount = 1},
							 	GBL_TYPE_FLAG_TYPEINFO_STATIC);
	}

	return type;
}

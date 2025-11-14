#include <gumball/gumball_manager.h>
#include <gumball/types/gumball_texture.h>
#include <gimbal/containers/gimbal_hash_set.h>
#include <gimbal/strings/gimbal_string_buffer.h>
#include <unistd.h>

// TODO: make these private variables!
static GblHashSet GUM_Manager_hashSet_;
static char GUM_Manager_currentPath_[1024];

typedef struct GUM_HashSetEntry {
    GUM_IResource *pResource;
    GblQuark quark;
} GUM_HashSetEntry;

static GblHash resourceHasher_(const GblHashSet* pSet, const void* pItem) {
    GBL_UNUSED(pSet);
    GUM_HashSetEntry *pEntry = (GUM_HashSetEntry*)pItem;
    return gblHash(&pEntry->quark, sizeof(GblQuark));
}

static GblBool resourceComparator_(const GblHashSet* pSelf, const void* pEntry1, const void* pEntry2) {
    GBL_UNUSED(pSelf);
    const GUM_HashSetEntry *pResEntry1 = (const GUM_HashSetEntry*)pEntry1;
    const GUM_HashSetEntry *pResEntry2 = (const GUM_HashSetEntry*)pEntry2;
    return pResEntry1->quark == pResEntry2->quark;
}

GUM_IResource *GUM_Manager_load(GblStringRef *path) {
    // TODO: LOG SHIT!!!!!!!!!!!!!!!!
    if (!path) return nullptr;

    GUM_IResource *pResource = nullptr;
    GblClass *managerClass = GblClass_refDefault(GUM_MANAGER_TYPE);

    GblStringBuffer stringBuffer;
    GblStringBuffer_construct(&stringBuffer, path);
    GblStringBuffer_prepend(&stringBuffer, "/");
    GblStringBuffer_prepend(&stringBuffer, GUM_Manager_currentPath_);

    GblStringRef *fullPath = GblStringBuffer_cString(&stringBuffer);
    GblQuark quark = GblQuark_fromString(fullPath);

    GUM_HashSetEntry entry = {
        .pResource = pResource,
        .quark = quark
    };

    if (GblHashSet_contains(&GUM_Manager_hashSet_, (const void*)&entry)) {
        entry = *(GUM_HashSetEntry*)GblHashSet_at(&GUM_Manager_hashSet_, &entry);
        goto end;
    }

    FILE *file = fopen(fullPath, "rb");
    if (!file) return nullptr;

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    GblByteArray *pData = GblByteArray_create(file_size);
    fread(GblByteArray_data(pData), 1, file_size, file);
    fclose(file);

    GblStringView stringView = GblStringView_fromString(fullPath);

    // TODO: supported extensions should be backend specific
    static const GblStringRef *texture_extensions[] = {
        ".png", ".bmp", ".tga", ".jpg", ".gif", ".hdr", ".pic",
        ".psd", ".dds", ".ktx", ".ktx2", ".pkm", ".pvr", ".astc"
    };

    // TODO: other resource types

    for (size_t i = 0; i < sizeof(texture_extensions) / sizeof(texture_extensions[0]); i++) {
        if (GblStringView_endsWith(stringView, texture_extensions[i])) {
            GUM_Texture *texture = GUM_TEXTURE(GblBox_create(GUM_TEXTURE_TYPE));
            GUM_IRESOURCE_CLASSOF(texture)->pFnCreate(GUM_IRESOURCE(texture), &pData, quark, texture_extensions[i]);
            GUM_Texture_loadFromBytes(texture);
            pResource = GUM_IRESOURCE(texture);
            entry.pResource = pResource;

            GblBool inserted = GblHashSet_insert(&GUM_Manager_hashSet_, (const void*)&entry);
            goto end;
            break;
        }
    }


    end:
    GblClass_unrefDefault(managerClass);
    GblStringBuffer_destruct(&stringBuffer);
    return GUM_IResource_ref(entry.pResource);
}

void GUM_Manager_unload(GUM_IResource *pResource) {
    if (!pResource) return;

    GblQuark quark = (GblQuark)GblBox_field(GBL_BOX(pResource), GblQuark_fromString("GUM_Resource_quark"));

    GUM_HashSetEntry entry = {
        .pResource = pResource,
        .quark = quark
    };

    if (GblHashSet_contains(&GUM_Manager_hashSet_, (const void*)&entry)) {
        GblHashSet_erase(&GUM_Manager_hashSet_, (const void*)&entry);
        GUM_IResource_unref(pResource);
    }

    if(GblBox_refCount(GBL_BOX(pResource)) == 0) {
        // free shit here
    }
}

GBL_RESULT GUM_ManagerClass_init_(GblClass* pClass, const void* pData) {
	GBL_UNUSED(pData);

    if (!GblType_classRefCount(GUM_MANAGER_TYPE)) {
        GblHashSet_construct(&GUM_Manager_hashSet_,
                             sizeof(GUM_HashSetEntry),
                             resourceHasher_,
                             resourceComparator_);
        getcwd(GUM_Manager_currentPath_, 1024);
    }

	return GBL_RESULT_SUCCESS;
}

GblType GUM_Manager_type(void) {
    static GblType type = GBL_INVALID_TYPE;

    if (type == GBL_INVALID_TYPE) {
        type = GblType_register(GblQuark_internStatic("GUM_Manager"),
                                GBL_STATIC_CLASS_TYPE,
                                &(static GblTypeInfo){.classSize = sizeof(GUM_ManagerClass),
                                                      .pFnClassInit = GUM_ManagerClass_init_},
                                GBL_TYPE_FLAG_TYPEINFO_STATIC | GBL_TYPE_FLAG_CLASS_PINNED);
    }

    return type;
}
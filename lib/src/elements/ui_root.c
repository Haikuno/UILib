#include <ui/elements/ui_widget.h>
#include <ui/elements/ui_root.h>

#include <gimbal/gimbal_algorithms.h>

static GblArrayList UI_drawQueue_;

static GBL_RESULT UI_Root_init_(GblInstance* pInstance) {
    GblObject_setName(GBL_OBJECT(pInstance), "UI_Root");
    if (!GblType_instanceCount(UI_ROOT_TYPE)) {
        GblModule_register(GBL_MODULE(pInstance));
    }
    return GBL_RESULT_SUCCESS;
}

static GBL_RESULT UI_RootClass_init_(GblClass* pClass, const void* pData) {
    GBL_UNUSED(pData);

    if (!GblType_classRefCount(UI_ROOT_TYPE)) {
        UI_drawQueue_init();
    }

    return GBL_RESULT_SUCCESS;
}

GblType UI_Root_type(void) {
    static GblType type = GBL_INVALID_TYPE;

    if (type == GBL_INVALID_TYPE) {
        type = GblType_register(GblQuark_internStatic("UI_Root"),
                                GBL_MODULE_TYPE,
                                &(static GblTypeInfo){.classSize = sizeof(UI_RootClass),
                                                      .instanceSize = sizeof(UI_Root),
                                                      .pFnClassInit = UI_RootClass_init_,
                                                      .pFnInstanceInit = UI_Root_init_},
                                GBL_TYPE_FLAG_TYPEINFO_STATIC);
    }

    return type;
}

UI_Root *UI_Root_create(void) {
    UI_Root *pRoot = GBL_NEW(UI_Root);
    GblModule_register(GBL_MODULE(pRoot));
    return pRoot;
}

static int UI_zIndex_cmp_(const void *pA, const void *pB) {
    GblObject *a = *(GblObject**)pA;
    GblObject *b = *(GblObject**)pB;

    UI_Widget *aWidget = GBL_AS(UI_Widget, a);
    UI_Widget *bWidget = GBL_AS(UI_Widget, b);

    if (! aWidget || ! bWidget) return 0;

    return aWidget->z_index - bWidget->z_index;
}

static void UI_drawQueue_sort_(void) {
    gblSortInsertion(GblArrayList_data(&UI_drawQueue_), GblArrayList_size(&UI_drawQueue_), sizeof(GblObject*), UI_zIndex_cmp_);
}

GblArrayList *UI_drawQueue_get(void) {
	return &UI_drawQueue_;
}

void UI_drawQueue_init(void) {
    GblArrayList_construct(&UI_drawQueue_, sizeof(GblObject*));
}

void UI_drawQueue_push(GblObject *pObj) {
    if (!pObj) return;
    UI_Widget *pWidget = GBL_AS(UI_Widget, pObj);
    if (!pWidget) return;

    GblArrayList_pushBack(&UI_drawQueue_, &pObj);
    UI_drawQueue_sort_();
}

void UI_drawQueue_remove(GblObject *pObj) {
    if (!pObj) return;
    if (!GBL_AS(UI_Widget, pObj)) return;

    for (size_t i = 0; i < GblArrayList_size(&UI_drawQueue_); i++) {
        GblObject *pObjQueue = *(GblObject**)GblArrayList_at(&UI_drawQueue_, i);
        if (pObjQueue == pObj) {
            GblArrayList_erase(&UI_drawQueue_, i, 1);
            UI_drawQueue_sort_();
            return;
        }
    }
}
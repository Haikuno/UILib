#include "ui_internal.h"

#include "ui_widget.h"

#include <gimbal/gimbal_algorithms.h>

static GblArrayList UI_drawQueue_;

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

GblArrayList *UI_getDrawQueue(void) {
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
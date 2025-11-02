#include <ui/elements/ui_widget.h>
#include <ui/elements/ui_root.h>
#include <ui/ui_general.h>

#include <gimbal/gimbal_containers.h>
#include <gimbal/gimbal_algorithms.h>

GBL_RESULT (UI_update)(GblObject* pSelf) {
    size_t childCount = GblObject_childCount(pSelf);

    GblObjectClass* pClass          = GBL_OBJECT_GET_CLASS(pSelf); // UI_TODO: change this to GBL_OBJECT_CLASSOF once libGimbal is updated
    UI_WidgetClass* pWidgetClass_   = GBL_CLASS_AS(UI_Widget, pClass);

    if GBL_UNLIKELY(pWidgetClass_) {
        pWidgetClass_->pFnUpdate(UI_WIDGET(pSelf));
    }

    for (size_t i = 0; i < childCount; i++) {
        GblObject* childObj = GblObject_findChildByIndex(pSelf, i);
        UI_update(childObj);
    }

    return GBL_RESULT_SUCCESS;
}

GBL_RESULT UI_draw(void) {
    size_t queueSize = GblArrayList_size(UI_drawQueue_get());

    for (size_t i = 0; i < queueSize; i++) {
        GblObject **pObj = GblArrayList_at(UI_drawQueue_get(), i);
        UI_WidgetClass *pWidgetClass = GBL_CLASS_AS(UI_Widget, GBL_OBJECT_GET_CLASS(*pObj));
        pWidgetClass->pFnDraw(UI_WIDGET(*pObj));
    }

    return GBL_RESULT_SUCCESS;
}


GBL_RESULT (UI_unref)(GblObject* pSelf) {
    UI_draw_disableAll(pSelf);

    UI_Widget *pWidget = GBL_AS(UI_Widget, pSelf);
    if (pWidget) {
        UI_WIDGET_CLASSOF(pSelf)->pFnDeactivate(pWidget);
    }

    size_t childCount = GblObject_childCount(pSelf);
    for (size_t i = childCount; i-- > 0;) {
        GblObject* childObj = GblObject_findChildByIndex(pSelf, i);
        if GBL_UNLIKELY(childObj) UI_unref(childObj);
    }

    GBL_UNREF(pSelf);

    return GBL_RESULT_SUCCESS;
}

void (UI_draw_disable)(GblObject *pSelf) {
    UI_drawQueue_remove(pSelf);
}

void (UI_draw_enable)(GblObject *pSelf) {
    UI_drawQueue_push(pSelf);
}

void (UI_draw_disableAll)(GblObject *pSelf) {
    size_t childCount = GblObject_childCount(pSelf);
    for (size_t i = 0; i < childCount; i++) {
        GblObject *childObj = GblObject_findChildByIndex(pSelf, i);
        UI_draw_disableAll(childObj);
    }
    UI_draw_disable(pSelf);
}

void (UI_draw_enableAll)(GblObject *pSelf) {
    UI_draw_enable(pSelf);
    size_t childCount = GblObject_childCount(pSelf);
    for (size_t i = childCount; i -- > 0;) {
        GblObject *childObj = GblObject_findChildByIndex(pSelf, i);
        UI_draw_enableAll(childObj);
    }
}
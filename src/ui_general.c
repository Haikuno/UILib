#include "ui_general.h"
#include "ui_widget.h"
#include "ui_internal.h"

#include <gimbal/gimbal_containers.h>
#include <gimbal/gimbal_algorithms.h>

GBL_RESULT UI_draw(void) {
    size_t queueSize = GblArrayList_size(UI_getDrawQueue());

    for (size_t i = 0; i < queueSize; i++) {
        GblObject **pObj = GblArrayList_at(UI_getDrawQueue(), i);
        UI_WidgetClass *pWidgetClass = GBL_CLASS_AS(UI_Widget, GBL_OBJECT_GET_CLASS(*pObj));
        pWidgetClass->pFnDraw(UI_WIDGET(*pObj));
    }

    return GBL_RESULT_SUCCESS;
}

void UI_draw_disable_(GblObject *pSelf) {
    UI_drawQueue_remove(pSelf);
}

void UI_draw_enable_(GblObject *pSelf) {
    UI_drawQueue_push(pSelf);
}

void UI_draw_disableAll_(GblObject *pSelf) {
    size_t childCount = GblObject_childCount(pSelf);
    for (size_t i = 0; i < childCount; i++) {
        GblObject *childObj = GblObject_findChildByIndex(pSelf, i);
        UI_draw_disableAll_(childObj);
    }
    UI_draw_disable_(pSelf);
}

void UI_draw_enableAll_(GblObject *pSelf) {
    UI_draw_enable_(pSelf);
    size_t childCount = GblObject_childCount(pSelf);
    for (size_t i = childCount; i -- > 0;) {
        GblObject *childObj = GblObject_findChildByIndex(pSelf, i);
        UI_draw_enableAll_(childObj);
    }
}

GBL_RESULT UI_update_(GblObject* pSelf) {
    size_t childCount = GblObject_childCount(pSelf);

    GblObjectClass* pClass          = GBL_OBJECT_GET_CLASS(pSelf); // UI_TODO: change this to GBL_OBJECT_CLASSOF once libGimbal is updated
    UI_WidgetClass* pWidgetClass_   = GBL_CLASS_AS(UI_Widget, pClass);

    if (pWidgetClass_ != nullptr) {
        pWidgetClass_->pFnUpdate(UI_WIDGET(pSelf));
    }

    for (size_t i = 0; i < childCount; i++) {
        GblObject* childObj = GblObject_findChildByIndex(pSelf, i);
        UI_update_(childObj);
    }

    return GBL_RESULT_SUCCESS;
}

GBL_RESULT UI_unref_(GblObject* pSelf) {
    UI_draw_disableAll_(pSelf);

    size_t      childCount  = GblObject_childCount(pSelf);
    for (size_t i = childCount; i-- > 0;) {
        GblObject* childObj = GblObject_findChildByIndex(pSelf, i);
        if (childObj != nullptr) UI_unref_(childObj);
    }

    GBL_UNREF(pSelf);

    return GBL_RESULT_SUCCESS;
}
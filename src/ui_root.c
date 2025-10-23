#include "ui_root.h"

#include "ui_buttonevent.h"
#include "ui_controller.h"
#include "ui_container.h"
#include "ui_widget.h"

#include <ctype.h>

#include <gimbal/gimbal_containers.h>
#include <gimbal/gimbal_algorithms.h>

static GblArrayList UI_drawQueue;

static void UI_drawQueue_init_(void) {
    printf("init draw queue\n");
    GblArrayList_construct(&UI_drawQueue, sizeof(GblObject*));
}

static int UI_zIndex_cmp_(const void *pA, const void *pB) {
    GblObject *a = *(GblObject**)pA;
    GblObject *b = *(GblObject**)pB;

    UI_Widget *aWidget = GBL_AS(UI_Widget, a);
    UI_Widget *bWidget = GBL_AS(UI_Widget, b);

    if (! aWidget || ! bWidget) return 0;

    int res = aWidget->z_index - bWidget->z_index;

    printf("zIndex: %d - %d = %d\n", aWidget->z_index, bWidget->z_index, res);

    return res;
}

void UI_drawQueue_sort_(void) {
    gblSortQuick(UI_drawQueue.private_.pData, GblArrayList_size(&UI_drawQueue), sizeof(GblObject*), UI_zIndex_cmp_);
    printf("sort ok!\n");
}

void UI_drawQueue_push_(GblObject *pObj) {
    if (!pObj) return;

    UI_Widget *pWidget = GBL_AS(UI_Widget, pObj);
    if (!pWidget) return;

    GblArrayList_pushBack(&UI_drawQueue, &pObj);

    UI_drawQueue_sort_();
}

void UI_drawQueue_remove_(GblObject *pObj) {
    if (!pObj) return;

    for (size_t i = 0; i < GblArrayList_size(&UI_drawQueue); i++) {
        if (GblArrayList_at(&UI_drawQueue, i) == pObj) {
            GblArrayList_erase(&UI_drawQueue, i, 1);
            return;
        }
    }

    UI_drawQueue_sort_();
}

static size_t GblObject_childIndex(GblObject *pSelf) {
    GblObject *pParent = GblObject_parent(pSelf);
    if (!pParent) return 0;

    size_t childCount = GblObject_childCount(pParent);
    for (size_t i = 0; i < childCount; i++) {
        GblObject *childObj = GblObject_findChildByIndex(pParent, i);
        if (childObj == pSelf) {
            return i;
        }
    }

    return 0;
}

static GblObject *GblObject_siblingPrevious(GblObject *pSelf) {
    GblObject *pParent = GblObject_parent(pSelf);
    if(!pParent) return nullptr;

    size_t childCount = GblObject_childCount(pParent);
    for (size_t i = 0; i < childCount; i++) {
        GblObject *childObj = GblObject_findChildByIndex(pParent, i);
        if (childObj == pSelf && i != 0) {
            return GblObject_findChildByIndex(pParent, i - 1);
        }
    }

    return nullptr;
}

static GblObject *GblObject_siblingPreviousByType(GblObject *pSelf, GblType type) {
    do pSelf = GblObject_siblingPrevious(pSelf);
    while (pSelf != nullptr && GBL_TYPEOF(pSelf) != type);
    return pSelf;
}

static GblObject *GblObject_siblingNextByType(GblObject *pSelf, GblType type) {
    do pSelf = GblObject_siblingNext(pSelf);
    while (pSelf != nullptr && GBL_TYPEOF(pSelf) != type);
    return pSelf;
}

static GblObject *GblObject_findDescendantByType(GblObject *pSelf, GblType descendantType) {
    GblArrayDeque queue;
    GblArrayDeque_construct(&queue, sizeof(GblObject*), 16, 0, nullptr, GBL_NULL);
    GblArrayDeque_pushBack(&queue, &pSelf);

    GblArrayList visited;
    GblArrayList_construct(&visited, sizeof(GblObject*));


    while (GblArrayDeque_size(&queue)) {
        GblObject **pObj = GblArrayDeque_popFront(&queue);

        size_t childCount = GblObject_childCount(*pObj);

        for (size_t i = 0; i < childCount; i++) {
            GblObject *childObj = GblObject_findChildByIndex(*pObj, i);

            for (size_t j = 0; j < GblArrayList_size(&visited); j++) {
                GblObject **pVisited = GblArrayList_at(&visited, j);
                if (*pVisited == childObj) {
                    continue;
                }
            }

            if (GBL_TYPEOF(childObj) == descendantType) {
                return childObj;
            }

            GblArrayDeque_pushBack(&queue, &childObj);
            GblArrayList_pushBack(&visited, &childObj);
        }
    }

    return nullptr;
}

// TODO: unreest! don't use chars, use enums
static UI_Button *move_cursor(GblObject *pSelf, UI_CONTROLLER_BUTTON buttonPress) {
    UI_Button *newButton = nullptr;
    GblObject *newObject = nullptr;

    GblObject *pParent = GblObject_parent(pSelf);
    if (pParent == nullptr || !GBL_AS(UI_Container, pParent)) return nullptr; // moving cursor around without a parent container is not supported yet!}
    const char parent_orientation = tolower(GBL_AS(UI_Container, pParent)->orientation);

    GblObject *pGrandParent = GblObject_parent(pParent);
    char grand_parent_orientation = 'N';
    if (GBL_AS(UI_Container, pGrandParent)) {
        grand_parent_orientation = tolower(GBL_AS(UI_Container, pGrandParent)->orientation);
    }

    size_t childCount = GblObject_childCount(pParent);
    size_t childIndex = GblObject_childIndex(pSelf);

    const char axis = buttonPress == UI_CONTROLLER_LEFT || buttonPress == UI_CONTROLLER_RIGHT ? 'h' : 'v'; // HORIZONTAL / VERTICAL
    const char dir  = buttonPress == UI_CONTROLLER_LEFT || buttonPress == UI_CONTROLLER_UP ? 'p' : 'n';    // PREVIOUS / NEXT


    if (axis != parent_orientation) {
        if (axis != grand_parent_orientation) {
            return nullptr;
        }

        if (dir == 'p') {
           goto MOVE_TO_PREVIOUS_CONTAINER;
        }

        if (dir == 'n') {
            goto MOVE_TO_NEXT_CONTAINER;
        }
    }

    if (axis == parent_orientation) {
        if (dir == 'p') {
            if (childIndex == 0) {
                if (axis == grand_parent_orientation) {
                    goto MOVE_TO_PREVIOUS_CONTAINER;
                }
            }

            // move to previous selectable button
            newObject = GblObject_siblingPreviousByType(pSelf, UI_BUTTON_TYPE);
            while (newObject != nullptr) {
                newButton = GBL_AS(UI_Button, newObject);
                if (newButton && newButton->isSelectable) return newButton;
                newObject = GblObject_siblingPreviousByType(newObject, UI_BUTTON_TYPE);
            }
        }

        if (dir == 'n') {
            if (childIndex == childCount - 1) {
                if (axis == grand_parent_orientation) {
                    goto MOVE_TO_NEXT_CONTAINER;
                }
            }

            // move to next selectable button
            newObject = GblObject_siblingNextByType(pSelf, UI_BUTTON_TYPE);
            while (newObject != nullptr) {
                newButton = GBL_AS(UI_Button, newObject);
                if (newButton && newButton->isSelectable) return newButton;
                newObject = GblObject_siblingNextByType(newObject, UI_BUTTON_TYPE);
            }
        }
    }

    return nullptr;

    GblObject *pNewContainer    = nullptr;
    GblObject *buttonCheck      = nullptr;

    MOVE_TO_PREVIOUS_CONTAINER:
        pNewContainer = GblObject_siblingPreviousByType(pParent, UI_CONTAINER_TYPE);
        if (!pNewContainer) return nullptr;

        buttonCheck = GblObject_findDescendantByType(pNewContainer, UI_BUTTON_TYPE);

        while (buttonCheck == nullptr) {
            pNewContainer = GblObject_siblingPreviousByType(pNewContainer, UI_CONTAINER_TYPE);
            if (!pNewContainer) return nullptr;
            buttonCheck = GblObject_findDescendantByType(pNewContainer, UI_BUTTON_TYPE);
        }

        goto GET_BUTTON_FROM_NEW_CONTAINER;

    MOVE_TO_NEXT_CONTAINER:
        pNewContainer = GblObject_siblingNextByType(pParent, UI_CONTAINER_TYPE);
        if (!pNewContainer) return nullptr;

        buttonCheck = GblObject_findDescendantByType(pNewContainer, UI_BUTTON_TYPE);

        while (buttonCheck == nullptr) {
            pNewContainer = GblObject_siblingNextByType(pNewContainer, UI_CONTAINER_TYPE);
            if (!pNewContainer) return nullptr;
            buttonCheck = GblObject_findDescendantByType(pNewContainer, UI_BUTTON_TYPE);
        }

        goto GET_BUTTON_FROM_NEW_CONTAINER;

    GET_BUTTON_FROM_NEW_CONTAINER:
        if (parent_orientation == grand_parent_orientation) {
            if (dir == 'p') {
                // we move to the last button in the new container
                newObject = GblObject_findDescendantByType(pNewContainer, UI_BUTTON_TYPE);
                newButton = GBL_AS(UI_Button, newObject);

                while (newObject != nullptr) {
                    newObject = GblObject_siblingNextByType(GBL_OBJECT(newObject), UI_BUTTON_TYPE);
                    if (newObject == nullptr) {
                        if (newButton != nullptr && newButton->isSelectable) {
                            return newButton;
                        }
                        return nullptr;
                    }

                    newButton = GBL_AS(UI_Button, newObject);
                }
                return nullptr;
            }

            if (dir == 'n') {
                // we move to the first button in the new container
                newObject = GblObject_findDescendantByType(pNewContainer, UI_BUTTON_TYPE);
                while (newObject != nullptr) {
                    newButton = GBL_AS(UI_Button, newObject);
                    if (newButton && newButton->isSelectable) return newButton;
                    newObject = GblObject_siblingNextByType(newObject, UI_BUTTON_TYPE);
                }
                return nullptr;
            }
        }

        if (parent_orientation != grand_parent_orientation) {

            if (UI_CONTAINER(pNewContainer)->orientation == parent_orientation) {
                // if the new container has enough buttons, we try to move to the same index
                if (GblObject_childCount(pNewContainer) > childIndex) {
                    newButton = GBL_AS(UI_Button, GblObject_findChildByIndex(pNewContainer, childIndex));

                    // if the new button is selectable, we move to it
                    if (newButton != nullptr && newButton->isSelectable) {
                        return newButton;
                    }

                    // otherwise, we move to the first previous selectable button in the new container
                    newObject = GblObject_siblingPreviousByType(GBL_OBJECT(newButton), UI_BUTTON_TYPE);
                    while (newObject != nullptr) {
                        newButton = GBL_AS(UI_Button, GblObject_siblingPreviousByType(GBL_OBJECT(newButton), UI_BUTTON_TYPE));
                        if (newButton != nullptr && newButton->isSelectable) {
                            return newButton;
                        }
                    }

                    // otherwise, we move to the first next selectable button in the new container
                    newObject = GblObject_siblingNextByType(GBL_OBJECT(newButton), UI_BUTTON_TYPE);
                    while (newObject != nullptr) {
                        newButton = GBL_AS(UI_Button, newObject);
                        if (newButton != nullptr && newButton->isSelectable) {
                            return newButton;
                        }
                    }

                    return nullptr;
                }
            }

            if(dir == 'p') {
                // we move to the last selectable button in the new container
                newObject = GblObject_findDescendantByType(pNewContainer, UI_BUTTON_TYPE);
                newButton = GBL_AS(UI_Button, newObject);

                while (newObject != nullptr) {
                    newObject = GblObject_siblingNextByType(GBL_OBJECT(newObject), UI_BUTTON_TYPE);
                    if (newObject == nullptr) {
                        if (newButton != nullptr && newButton->isSelectable) {
                            return newButton;
                        }
                        return nullptr;
                    }

                    newButton = GBL_AS(UI_Button, newObject);
                }
                return nullptr;
            }

            if (dir == 'n') {
                // we move to the first selectable button in the new container
                newObject = GblObject_findDescendantByType(pNewContainer, UI_BUTTON_TYPE);
                while (newObject != nullptr) {
                    newButton = GBL_AS(UI_Button, newObject);
                    if (newButton && newButton->isSelectable) return newButton;
                    newObject = GblObject_siblingNextByType(newObject, UI_BUTTON_TYPE);
                }
                return nullptr;
            }


        }

        return nullptr;

    return nullptr;
}

static GBL_RESULT UI_Root_handle_event_(GblIEventHandler* pSelf, GblEvent* pEvent) {
    GblEvent_accept(pEvent);

    UI_ButtonEvent	*buttonEvent	= (UI_ButtonEvent*)pEvent;
    UI_Controller	*pController	= UI_CONTROLLER(GblBox_userdata(GBL_BOX(pEvent)));

    UI_Button		**pPButton		= &pController->pSelectedButton;

    const bool isDirection         =    buttonEvent->button == UI_CONTROLLER_UP   ||
                                        buttonEvent->button == UI_CONTROLLER_DOWN ||
                                        buttonEvent->button == UI_CONTROLLER_LEFT ||
                                        buttonEvent->button == UI_CONTROLLER_RIGHT;

    const bool isPress             =    buttonEvent->state  == UI_CONTROLLER_BUTTON_PRESS;

    const bool isDirectionPress    =    isDirection && isPress;

    if (isDirectionPress) {
        UI_Button *newButton = nullptr;

        if (*pPButton == nullptr) {
            newButton = GBL_AS(UI_Button, GblObject_findDescendantByType(GBL_OBJECT(pSelf), UI_BUTTON_TYPE));
            *pPButton = newButton;
            (*pPButton)->isSelected = true;
            return GBL_RESULT_SUCCESS;
        }

        newButton = move_cursor(GBL_OBJECT(*pPButton), buttonEvent->button);

        if (newButton != nullptr) {
            (*pPButton)->isSelected = false;
            *pPButton = newButton;
            (*pPButton)->isSelected = true;
        }
    }

    if (*pPButton != nullptr && isPress && !isDirection) {
        if (pController->isKeyboard) printf("keyboard ");
        else printf("controller ");
        printf("with id %i pressed button %s!\n", pController->controllerId, GblStringBuffer_cString(&UI_WIDGET((*pPButton))->label));

        switch (buttonEvent->button) {
            case UI_CONTROLLER_PRIMARY:
                GBL_EMIT(*pPButton, "onPressPrimary");
                break;
            case UI_CONTROLLER_SECONDARY:
                GBL_EMIT(*pPButton, "onPressSecondary");
                break;
            case UI_CONTROLLER_TERTIARY:
                GBL_EMIT(*pPButton, "onPressTertiary");
                break;
            default:
                break;
        }
    }

    return GBL_RESULT_SUCCESS;
}

static GBL_RESULT UI_RootClass_init_(GblClass* pClass, const void* pData) {
    GBL_UNUSED(pData);
    UI_ROOT_CLASS(pClass)->base.GblIEventHandlerImpl.pFnEvent = UI_Root_handle_event_;

    if (!GblType_classRefCount(GBL_CLASS_TYPEOF(pClass))) {
        UI_drawQueue_init_();
    }

    return GBL_RESULT_SUCCESS;
}

static GBL_RESULT UI_Root_init_(GblInstance* pInstance) { return GBL_RESULT_SUCCESS; }

GblType UI_Root_type(void) {
    static GblType type = GBL_INVALID_TYPE;

    if (type == GBL_INVALID_TYPE) {
        type = GblType_register(GblQuark_internStatic("UI_Root"), GBL_OBJECT_TYPE, &(static GblTypeInfo){.classSize = sizeof(UI_RootClass), .pFnClassInit = UI_RootClass_init_, .instanceSize = sizeof(UI_Root), .pFnInstanceInit = UI_Root_init_},
                                GBL_TYPE_FLAG_TYPEINFO_STATIC);
    }

    return type;
}

/// General UI functions ///

GBL_RESULT UI_update_(GblObject* pSelf) {
    size_t childCount = GblObject_childCount(pSelf);

    GblObjectClass* pClass          = GBL_OBJECT_GET_CLASS(pSelf);
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

GBL_RESULT UI_draw(void) {

    size_t queueSize = GblArrayList_size(&UI_drawQueue);

    for (size_t i = 0; i < queueSize; i++) {
        GblObject **pObj = GblArrayList_at(&UI_drawQueue, i);

        UI_WidgetClass *pWidgetClass = GBL_CLASS_AS(UI_Widget, GBL_OBJECT_GET_CLASS(*pObj));

        pWidgetClass->pFnDraw(UI_WIDGET(*pObj));
    }

    return GBL_RESULT_SUCCESS;
}

GBL_RESULT UI_unref_(GblObject* pSelf) {
    size_t      childCount  = GblObject_childCount(pSelf);

    for (size_t i = childCount; i-- > 0;) {
        GblObject* childObj = GblObject_findChildByIndex(pSelf, i);
        if (childObj != nullptr) UI_unref_(childObj);
    }

    GBL_UNREF(pSelf);

    return GBL_RESULT_SUCCESS;
}
#include "ui_root.h"

#include "ui_buttonevent.h"
#include "ui_controller.h"
#include "ui_internal.h"
#include "ui_container.h"
#include "ui_general.h"
#include "ui_widget.h"

#include <gimbal/gimbal_containers.h>

static GblObject *GblObject_findDescendantByType(GblObject *pSelf, GblType descendantType) {
    if (!pSelf) return nullptr;

    GblArrayDeque queue;
    GblArrayDeque_construct(&queue, sizeof(GblObject*), 8);
    GblArrayDeque_pushBack(&queue, &pSelf);

    while (GblArrayDeque_size(&queue)) {
        GblObject **ppObj = GblArrayDeque_popFront(&queue);
        size_t childCount = GblObject_childCount(*ppObj);

        for (size_t i = 0; i < childCount; i++) {
            GblObject *childObj = GblObject_findChildByIndex(*ppObj, i);
            if (GBL_TYPEOF(childObj) == descendantType) {
                GblArrayDeque_destruct(&queue);
                return childObj;
            }
            GblArrayDeque_pushBack(&queue, &childObj);
        }
    }

    GblArrayDeque_destruct(&queue);
    return nullptr;
}

static UI_Button* findSelectableSibling(GblObject* pObj, bool next) {
    GblObject* sibling = next  ? GblObject_siblingNextByType(pObj, UI_BUTTON_TYPE)
                               : GblObject_siblingPreviousByType(pObj, UI_BUTTON_TYPE);
    while (sibling) {
        UI_Button* button = GBL_AS(UI_Button, sibling);
        if (button && button->isSelectable) return button;
        sibling = next ? GblObject_siblingNextByType(sibling, UI_BUTTON_TYPE)
                       : GblObject_siblingPreviousByType(sibling, UI_BUTTON_TYPE);
    }
    return nullptr;
}

static GblObject* findSiblingContainerWithButton(GblObject* pContainer, bool next) {
    GblObject* sibling = next  ? GblObject_siblingNextByType(pContainer, UI_CONTAINER_TYPE)
                               : GblObject_siblingPreviousByType(pContainer, UI_CONTAINER_TYPE);
    while (sibling) {
        if (GblObject_findDescendantByType(sibling, UI_BUTTON_TYPE)) return sibling;
        sibling = next ? GblObject_siblingNextByType(sibling, UI_CONTAINER_TYPE)
                       : GblObject_siblingPreviousByType(sibling, UI_CONTAINER_TYPE);
    }
    return nullptr;
}

static UI_Button* findSelectableInContainer(GblObject* pContainer, bool last, size_t preferredIndex) {
    GblObject* obj = GblObject_findDescendantByType(pContainer, UI_BUTTON_TYPE);
    if (!obj) return nullptr;

    UI_Button* candidate = nullptr;
    size_t index = 0;

    while (obj) {
        UI_Button* button = GBL_AS(UI_Button, obj);
        if (button && button->isSelectable) {
            if (preferredIndex != GBL_INDEX_INVALID && index == preferredIndex) return button;
            candidate = button;
        }
        obj = GblObject_siblingNextByType(obj, UI_BUTTON_TYPE);
        index++;
    }

    // If no preferred index match, return first (if !last) or last (if last) selectable
    if (last && candidate) return candidate;
    if (!last) {
        obj = GblObject_findDescendantByType(pContainer, UI_BUTTON_TYPE);
        while (obj) {
            UI_Button* button = GBL_AS(UI_Button, obj);
            if (button && button->isSelectable) return button;
            obj = GblObject_siblingNextByType(obj, UI_BUTTON_TYPE);
        }
    }
    return nullptr;
}

static UI_Button* moveCursor(GblObject* pSelf, UI_CONTROLLER_BUTTON buttonPress) {
    GblObject* pParent = GblObject_parent(pSelf);
    if (!pParent || !GBL_AS(UI_Container, pParent)) return nullptr;

    char parent_orientation = tolower(GBL_AS(UI_Container, pParent)->orientation);
    GblObject* pGrandParent = GblObject_parent(pParent);
    char grand_parent_orientation = (pGrandParent && GBL_AS(UI_Container, pGrandParent)) ?
                                    tolower(GBL_AS(UI_Container, pGrandParent)->orientation) : 'N';

    char axis = (buttonPress == UI_CONTROLLER_LEFT  || buttonPress == UI_CONTROLLER_RIGHT) ? 'h' : 'v';
    bool next = (buttonPress == UI_CONTROLLER_RIGHT || buttonPress == UI_CONTROLLER_DOWN);

    if (axis != parent_orientation && axis != grand_parent_orientation) return nullptr;

    size_t childIndex = GblObject_childIndex(pSelf);

    // Intra-container movement if axis matches parent
    if (axis == parent_orientation) {
        UI_Button* sibling = findSelectableSibling(pSelf, next);
        if (sibling) return sibling;

        // If axis doesn't match grandparent, inter-container movement is not possible
        if (axis != grand_parent_orientation) return nullptr;
    }

    // Inter-container movement
    GblObject* pNewContainer = findSiblingContainerWithButton(pParent, next);
    if (!pNewContainer) return nullptr;

    char new_orientation = tolower(GBL_AS(UI_Container, pNewContainer)->orientation);
    size_t preferredIndex = (new_orientation == parent_orientation && GblObject_childCount(pNewContainer) > childIndex)
                            ? childIndex : GBL_INDEX_INVALID;

    // If orientations match at parent/grandparent level, use first/last, else try preferred index or fallback
    bool useLast = !next;
    if (parent_orientation == grand_parent_orientation) useLast = !next;
    return findSelectableInContainer(pNewContainer, useLast, preferredIndex);
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

        newButton = moveCursor(GBL_OBJECT(*pPButton), buttonEvent->button);

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

    if (!GblType_classRefCount(UI_ROOT_TYPE)) {
        UI_drawQueue_init();
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
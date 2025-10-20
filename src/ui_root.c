#include "ui_root.h"

#include "ui_buttonevent.h"
#include "ui_controller.h"
#include "ui_container.h"
#include "ui_widget.h"

#include <ctype.h>

#include <gimbal/gimbal_containers.h>

static size_t get_child_index(GblObject *pSelf) {
    GblObject *pParent = GblObject_parent(pSelf);
    if (!pParent) return 0;

    size_t child_count = GblObject_childCount(pParent);
    for (size_t i = 0; i < child_count; i++) {
        GblObject *child_obj = GblObject_findChildByIndex(pParent, i);
        if (child_obj == pSelf) {
            return i;
        }
    }

    return 0;
}

static GblObject *find_previous_sibling(GblObject *pSelf) {
    GblObject *pParent = GblObject_parent(pSelf);
    if(!pParent) return nullptr;

    size_t child_count = GblObject_childCount(pParent);
    for (size_t i = 0; i < child_count; i++) {
        GblObject *child_obj = GblObject_findChildByIndex(pParent, i);
        if (child_obj == pSelf && i != 0) {
            return GblObject_findChildByIndex(pParent, i - 1);
        }
    }

    return nullptr;
}

static GblObject *find_previous_sibling_of_type(GblObject *pSelf, GblType type) {
    GblObject *pObj = pSelf;

    do pObj = find_previous_sibling(pObj);
    while (pObj != nullptr && GBL_TYPEOF(pObj) != type);

    return pObj;
}

static GblObject *find_next_sibling(GblObject *pSelf) {
    return GblObject_siblingNext(pSelf);
}

static GblObject *find_next_sibling_of_type(GblObject *pSelf, GblType type) {
    GblObject *pObj = pSelf;

    do pObj = find_next_sibling(pObj);
    while (pObj != nullptr && GBL_TYPEOF(pObj) != type);

    return pObj;
}

static GblObject *find_descendant_of_type(GblObject *pSelf, GblType descendantType) {
    GblArrayDeque queue;
    GblArrayDeque_construct(&queue, sizeof(GblObject*), 16, 0, nullptr, GBL_NULL);
    GblArrayDeque_pushBack(&queue, &pSelf);

    GblArrayList visited;
    GblArrayList_construct(&visited, sizeof(GblObject*));


    while (GblArrayDeque_size(&queue)) {
        GblObject **pObj = GblArrayDeque_popFront(&queue);

        size_t child_count = GblObject_childCount(*pObj);

        for (size_t i = 0; i < child_count; i++) {
            GblObject *child_obj = GblObject_findChildByIndex(*pObj, i);

            for (size_t j = 0; j < GblArrayList_size(&visited); j++) {
                GblObject **pVisited = GblArrayList_at(&visited, j);
                if (*pVisited == child_obj) {
                    continue;
                }
            }

            if (GBL_TYPEOF(child_obj) == descendantType) {
                return child_obj;
            }

            GblArrayDeque_pushBack(&queue, &child_obj);
            GblArrayList_pushBack(&visited, &child_obj);
        }
    }

    return nullptr;
}

// TODO: unreest! don't use chars, use enums
static UI_Button *move_cursor(GblObject *pSelf, UI_CONTROLLER_BUTTON button_press) {
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

    size_t child_count = GblObject_childCount(pParent);
    size_t child_index = get_child_index(pSelf);

    const char axis = button_press == UI_CONTROLLER_LEFT || button_press == UI_CONTROLLER_RIGHT ? 'h' : 'v'; // HORIZONTAL / VERTICAL
    const char dir  = button_press == UI_CONTROLLER_LEFT || button_press == UI_CONTROLLER_UP ? 'p' : 'n';    // PREVIOUS / NEXT


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
            if (child_index == 0) {
                if (axis == grand_parent_orientation) {
                    goto MOVE_TO_PREVIOUS_CONTAINER;
                }
            }

            // move to previous selectable button
            newObject = find_previous_sibling_of_type(pSelf, UI_BUTTON_TYPE);
            while (newObject != nullptr) {
                newButton = GBL_AS(UI_Button, newObject);
                if (newButton && newButton->is_selectable) return newButton;
                newObject = find_previous_sibling_of_type(newObject, UI_BUTTON_TYPE);
            }
        }

        if (dir == 'n') {
            if (child_index == child_count - 1) {
                if (axis == grand_parent_orientation) {
                    goto MOVE_TO_NEXT_CONTAINER;
                }
            }

            // move to next selectable button
            newObject = find_next_sibling_of_type(pSelf, UI_BUTTON_TYPE);
            while (newObject != nullptr) {
                newButton = GBL_AS(UI_Button, newObject);
                if (newButton && newButton->is_selectable) return newButton;
                newObject = find_next_sibling_of_type(newObject, UI_BUTTON_TYPE);
            }
        }
    }

    return nullptr;

    GblObject *pNewContainer = nullptr;

    MOVE_TO_PREVIOUS_CONTAINER:
        pNewContainer = find_previous_sibling_of_type(pParent, UI_CONTAINER_TYPE);
        if (!pNewContainer) return nullptr;
        goto GET_BUTTON_FROM_NEW_CONTAINER;

    MOVE_TO_NEXT_CONTAINER:
        pNewContainer = find_next_sibling_of_type(pParent, UI_CONTAINER_TYPE);
        if (!pNewContainer) return nullptr;
        goto GET_BUTTON_FROM_NEW_CONTAINER;

    GET_BUTTON_FROM_NEW_CONTAINER:

        if (parent_orientation == grand_parent_orientation) {
            if (dir == 'p') {
                // we move to the last button in the new container
                newObject = find_descendant_of_type(pNewContainer, UI_BUTTON_TYPE);
                newButton = GBL_AS(UI_Button, newObject);

                while (newObject != nullptr) {
                    newObject = find_next_sibling_of_type(GBL_OBJECT(newObject), UI_BUTTON_TYPE);
                    if (newObject == nullptr) {
                        if (newButton != nullptr && newButton->is_selectable) {
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
                newObject = find_descendant_of_type(pNewContainer, UI_BUTTON_TYPE);
                while (newObject != nullptr) {
                    newButton = GBL_AS(UI_Button, newObject);
                    if (newButton && newButton->is_selectable) return newButton;
                    newObject = find_next_sibling_of_type(newObject, UI_BUTTON_TYPE);
                }
                return nullptr;
            }
        }

        if (parent_orientation != grand_parent_orientation) {
            // if the new container has enough buttons, we try to move to the same index
            if (GblObject_childCount(pNewContainer) > child_index) {
                newButton = GBL_AS(UI_Button, GblObject_findChildByIndex(pNewContainer, child_index));

                // if the new button is selectable, we move to it
                if (newButton != nullptr && newButton->is_selectable) {
                    return newButton;
                }

                // otherwise, we move to the first previous selectable button in the new container
                newObject = find_previous_sibling_of_type(GBL_OBJECT(newButton), UI_BUTTON_TYPE);
                while (newObject != nullptr) {
                    newButton = GBL_AS(UI_Button, find_previous_sibling_of_type(GBL_OBJECT(newButton), UI_BUTTON_TYPE));
                    if (newButton != nullptr && newButton->is_selectable) {
                        return newButton;
                    }
                }

                // otherwise, we move to the first next selectable button in the new container
                newObject = find_next_sibling_of_type(GBL_OBJECT(newButton), UI_BUTTON_TYPE);
                while (newObject != nullptr) {
                    newButton = GBL_AS(UI_Button, newObject);
                    if (newButton != nullptr && newButton->is_selectable) {
                        return newButton;
                    }
                }

                return nullptr;
            }

            // otherwise, we move to the last button in the new container
            newObject = find_descendant_of_type(pNewContainer, UI_BUTTON_TYPE);
            newButton = GBL_AS(UI_Button, newObject);

            while (newObject != nullptr) {
                newObject = find_next_sibling_of_type(GBL_OBJECT(newObject), UI_BUTTON_TYPE);
                if (newObject == nullptr) {
                    if (newButton != nullptr && newButton->is_selectable) {
                        return newButton;
                    }
                    return nullptr;
                }

                newButton = GBL_AS(UI_Button, newObject);
            }
            return nullptr;
        }

        return nullptr;

    return nullptr;
}

static GBL_RESULT UI_Root_handle_event_(GblIEventHandler* pSelf, GblEvent* pEvent) {
    GblEvent_accept(pEvent);

    UI_ButtonEvent	*button_event	= (UI_ButtonEvent*)pEvent;
    UI_Controller	*pController	= UI_CONTROLLER(GblBox_userdata(GBL_BOX(pEvent)));
    UI_Button		**pPButton		= &pController->pSelectedButton;

    const bool is_direction         =   button_event->button == UI_CONTROLLER_UP   ||
                                        button_event->button == UI_CONTROLLER_DOWN ||
                                        button_event->button == UI_CONTROLLER_LEFT ||
                                        button_event->button == UI_CONTROLLER_RIGHT;

    const bool is_press             =   button_event->state  == UI_CONTROLLER_BUTTON_PRESS;

    const bool is_direction_press   =   is_direction && is_press;

    if (is_direction_press) {
        UI_Button *newButton = nullptr;

        if (*pPButton == nullptr) {
            newButton = GBL_AS(UI_Button, find_descendant_of_type(GBL_OBJECT(pSelf), UI_BUTTON_TYPE));
            *pPButton = newButton;
            (*pPButton)->is_selected = true;
            return GBL_RESULT_SUCCESS;
        }

        newButton = move_cursor(GBL_OBJECT(*pPButton), button_event->button);

        if (newButton != nullptr) {
            (*pPButton)->is_selected = false;
            *pPButton = newButton;
            (*pPButton)->is_selected = true;
        }
    }

    if (is_press && *pPButton != nullptr && button_event->button == UI_CONTROLLER_PRIMARY) {
        GBL_EMIT(*pPButton, "on_press");
    }

    return GBL_RESULT_SUCCESS;
}

static GBL_RESULT UI_RootClass_init_(GblClass* pClass, const void* pData) {
    GBL_UNUSED(pData);
    UI_ROOT_CLASS(pClass)->base.GblIEventHandlerImpl.pFnEvent = UI_Root_handle_event_;
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
    size_t child_count = GblObject_childCount(pSelf);

    GblObjectClass* pClass  = GBL_OBJECT_GET_CLASS(pSelf);
    UI_WidgetClass* pClass_ = GBL_CLASS_AS(UI_Widget, pClass);

    if (pClass_ != nullptr) {
        pClass_->pFnUpdate(UI_WIDGET(pSelf));
    }

    for (size_t i = 0; i < child_count; i++) {
        GblObject* child_obj = GblObject_findChildByIndex(pSelf, i);
        UI_update_(child_obj);
    }

    return GBL_RESULT_SUCCESS;
}

GBL_RESULT UI_draw_(GblObject* pSelf) {
    size_t child_count = GblObject_childCount(pSelf);

    GblObjectClass* pClass  = GBL_OBJECT_GET_CLASS(pSelf);
    UI_WidgetClass* pClass_ = GBL_CLASS_AS(UI_Widget, pClass);

    if (pClass_ != nullptr) {
        pClass_->pFnDraw(UI_WIDGET(pSelf));
    }

    for (size_t i = 0; i < child_count; i++) {
        GblObject* child_obj = GblObject_findChildByIndex(pSelf, i);
        UI_draw_(child_obj);
    }

    return GBL_RESULT_SUCCESS;
}

GBL_RESULT UI_unref_(GblObject* pSelf) {
    size_t      child_count = GblObject_childCount(pSelf);
    const char* name        = GblType_name(GBL_TYPEOF(pSelf));

    for (size_t i = child_count; i-- > 0;) {
        GblObject* child_obj = GblObject_findChildByIndex(pSelf, i);
        if (child_obj != nullptr) UI_unref_(child_obj);
    }

    GBL_UNREF(pSelf);

    return GBL_RESULT_SUCCESS;
}
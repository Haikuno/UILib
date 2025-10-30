#include "ui_controller.h"
#include "ui_container.h"
#include "ui_root.h"


#include <gimbal/gimbal_containers.h>

#include <raylib.h>

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

static UI_Button* findSelectableSibling_(GblObject* pObj, bool next) {
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

static GblObject* findSiblingContainerWithButton_(GblObject* pContainer, bool next) {
    GblObject* sibling = next  ? GblObject_siblingNextByType(pContainer, UI_CONTAINER_TYPE)
                               : GblObject_siblingPreviousByType(pContainer, UI_CONTAINER_TYPE);
    while (sibling) {
        if (GblObject_findDescendantByType(sibling, UI_BUTTON_TYPE)) return sibling;
        sibling = next ? GblObject_siblingNextByType(sibling, UI_CONTAINER_TYPE)
                       : GblObject_siblingPreviousByType(sibling, UI_CONTAINER_TYPE);
    }
    return nullptr;
}

static UI_Button* findSelectableInContainer_(GblObject* pContainer, bool last, size_t preferredIndex) {
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

static UI_Button* moveCursor_(GblObject* pSelf, UI_CONTROLLER_BUTTON buttonPress) {
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
        UI_Button* sibling = findSelectableSibling_(pSelf, next);
        if (sibling) return sibling;
    }

    // Inter-container movement
    GblObject* pNewContainer = nullptr;

    // If axis doesn't match grandparent, inter-container movement should be attempted with grandparent's siblings
    if (axis != grand_parent_orientation) {
        pNewContainer = findSiblingContainerWithButton_(pGrandParent, next);
    } else {
        pNewContainer = findSiblingContainerWithButton_(pParent, next);
    }

    if (!pNewContainer) return nullptr;

    // Make sure the new container has buttons, and not just containers that contain buttons
    auto firstChild = GblObject_childFirst(pNewContainer);
    if (!findSelectableSibling_(firstChild, true)) {
        pNewContainer = firstChild;
    }

    char new_orientation = tolower(GBL_AS(UI_Container, pNewContainer)->orientation);
    size_t preferredIndex = (new_orientation == parent_orientation && GblObject_childCount(pNewContainer) > childIndex)
                            ? childIndex : GBL_INDEX_INVALID;

    // If orientations match at parent/grandparent level, use first/last, else try preferred index or fallback
    bool useLast = !next;
    if (parent_orientation == grand_parent_orientation) useLast = !next;
    return findSelectableInContainer_(pNewContainer, useLast, preferredIndex);
}

static GBL_RESULT UI_Controller_handleButton_(UI_Controller *pSelf, UI_CONTROLLER_BUTTON_STATE eventState, UI_CONTROLLER_BUTTON eventButton) {
    UI_Button		**ppButton		= &pSelf->pSelectedButton;
    auto            root            = GBL_REQUIRE(UI_Root, "UI_Root");

    const bool isDirection         =    eventButton == UI_CONTROLLER_UP   ||
                                        eventButton == UI_CONTROLLER_DOWN ||
                                        eventButton == UI_CONTROLLER_LEFT ||
                                        eventButton == UI_CONTROLLER_RIGHT;

    const bool isPress             =    eventState  == UI_CONTROLLER_BUTTON_PRESS;

    const bool isDirectionPress    =    isDirection && isPress;

    if (isDirectionPress) {
        UI_Button *newButton = nullptr;

        if (*ppButton == nullptr) {
            newButton = GBL_AS(UI_Button, GblObject_findDescendantByType(GBL_OBJECT(root), UI_BUTTON_TYPE));
            if (!newButton) return GBL_RESULT_ERROR;
            *ppButton = newButton;
            (*ppButton)->isSelected = true;
            return GBL_RESULT_SUCCESS;
        }

        newButton = moveCursor_(GBL_OBJECT(*ppButton), eventButton);

        if (newButton != nullptr) {
            (*ppButton)->isSelected = false;
            *ppButton = newButton;
            (*ppButton)->isSelected = true;
        }
    }

    if (*ppButton != nullptr && isPress && !isDirection) {
        switch (eventButton) {
            case UI_CONTROLLER_PRIMARY:
                GBL_EMIT(*ppButton, "onPressPrimary");
                break;
            case UI_CONTROLLER_SECONDARY:
                GBL_EMIT(*ppButton, "onPressSecondary");
                break;
            case UI_CONTROLLER_TERTIARY:
                GBL_EMIT(*ppButton, "onPressTertiary");
                break;
            default:
                break;
        }
    }

    return GBL_RESULT_SUCCESS;
}

static GBL_RESULT UI_Controller_GblObject_setProperty_(GblObject *pObject, const GblProperty *pProp, GblVariant *pValue) {
	UI_Controller *pSelf = UI_CONTROLLER(pObject);

	switch (pProp->id) {
		case UI_Controller_Property_Id_controllerId:
			GblVariant_valueCopy(pValue, &pSelf->controllerId);
			break;
		case UI_Controller_Property_Id_isKeyboard:
			GblVariant_valueCopy(pValue, &pSelf->isKeyboard);
			break;
		default:
			return GBL_RESULT_ERROR_INVALID_PROPERTY;
	}

	return GBL_RESULT_SUCCESS;
}

static GBL_RESULT UI_Controller_GblObject_property_(const GblObject *pObject, const GblProperty *pProp, GblVariant *pValue) {
	UI_Controller *pSelf = UI_CONTROLLER(pObject);

	switch (pProp->id) {
		case UI_Controller_Property_Id_controllerId:
			GblVariant_setUint8(pValue, pSelf->controllerId);
			break;
		case UI_Controller_Property_Id_isKeyboard:
			GblVariant_setBool(pValue, pSelf->isKeyboard);
			break;
		default:
			return GBL_RESULT_ERROR_INVALID_PROPERTY;
	}

	return GBL_RESULT_SUCCESS;
}

static UI_Button *findDefaultSelectableInContainer_(UI_Container *pContainer) {
    UI_Button *pButton = GBL_AS(UI_Button, GblObject_findDescendantByType(GBL_OBJECT(pContainer), UI_BUTTON_TYPE));
    while (pButton) {
        if (pButton->isSelectedByDefault) return pButton;
        pButton = GBL_AS(UI_Button, GblObject_siblingNextByType(GBL_OBJECT(pButton), UI_BUTTON_TYPE));
    }
    return nullptr;
}

static GBL_RESULT UI_Controller_update_(UI_Widget *pSelf) {
    // find first selected by default button
    if (!UI_CONTROLLER(pSelf)->pSelectedButton) {
        auto root = GBL_REQUIRE(UI_Root, "UI_Root");

        UI_Container *pContainer = GBL_AS(UI_Container, GblObject_findDescendantByType(GBL_OBJECT(root), UI_CONTAINER_TYPE));
        UI_Button    *pButton    = findDefaultSelectableInContainer_(pContainer);

        while (pContainer) {
            if (pButton) {
                UI_CONTROLLER(pSelf)->pSelectedButton = pButton;
                return GBL_RESULT_SUCCESS;
            }

            auto pNextContainer = GBL_AS(UI_Container, GblObject_siblingNextByType(GBL_OBJECT(pContainer), UI_CONTAINER_TYPE));
            if (!pNextContainer) {
                pNextContainer = GBL_AS(UI_Container, GblObject_findDescendantByType(GBL_OBJECT(pContainer), UI_CONTAINER_TYPE));
                if (!pNextContainer) break;
            }
            pContainer = pNextContainer;
            pButton    = findDefaultSelectableInContainer_(pContainer);
        }
    }

    return GBL_RESULT_SUCCESS;
}

static GBL_RESULT UI_Controller_draw_(UI_Widget *pSelf) {
	UI_Controller *pController = UI_CONTROLLER(pSelf);

	if (pController->pSelectedButton) {
		Rectangle rec = {
			.x = UI_WIDGET(pController->pSelectedButton)->x,
			.y = UI_WIDGET(pController->pSelectedButton)->y,
			.width  = UI_WIDGET(pController->pSelectedButton)->w,
			.height = UI_WIDGET(pController->pSelectedButton)->h
		};

		DrawRectangleRoundedLinesEx(rec, pSelf->border_radius, 6, pSelf->border_width * 0.75, (Color){ pSelf->r, pSelf->g, pSelf->b, pSelf->a });
	}

	return GBL_RESULT_SUCCESS;
}

static GBL_RESULT UI_ControllerClass_init_(GblClass *pClass, const void *pData) {
	GBL_UNUSED(pData);

	if (!GblType_classRefCount(UI_CONTROLLER_TYPE)) GBL_PROPERTIES_REGISTER(UI_Controller);

	GBL_OBJECT_CLASS(pClass)->pFnSetProperty 	= UI_Controller_GblObject_setProperty_;
	GBL_OBJECT_CLASS(pClass)->pFnProperty    	= UI_Controller_GblObject_property_;


    UI_WIDGET_CLASS(pClass)->pFnDraw       		= UI_Controller_draw_;
	UI_WIDGET_CLASS(pClass)->pFnUpdate			= UI_Controller_update_;

	return GBL_RESULT_SUCCESS;
}

static GBL_RESULT UI_Controller_init_(GblInstance *pInstance) {
	UI_CONTROLLER(pInstance)->controllerId 		= 0;
	UI_CONTROLLER(pInstance)->isKeyboard 		= false;

	UI_WIDGET(pInstance)->r 					= 0;
	UI_WIDGET(pInstance)->g 					= 0;
	UI_WIDGET(pInstance)->b 					= 0;
	UI_WIDGET(pInstance)->a 					= 255;
	UI_WIDGET(pInstance)->z_index 				= 200;

	return GBL_RESULT_SUCCESS;
}

void UI_Controller_sendButton(UI_Controller *pSelf, UI_CONTROLLER_BUTTON_STATE state, UI_CONTROLLER_BUTTON button) {
    UI_Controller_handleButton_(pSelf, state, button);
}

void UI_Controller_setSelectedButton(UI_Controller *pSelf, UI_Button* pButton) {
	UI_CONTROLLER(pSelf)->pSelectedButton = pButton;
}

GblType UI_Controller_type(void) {
	static GblType type = GBL_INVALID_TYPE;

	if (type == GBL_INVALID_TYPE) {
		type =
			GblType_register(GblQuark_internStatic("UI_Controller"),
							 UI_WIDGET_TYPE,
							 &(static GblTypeInfo){
													.classSize			= sizeof(UI_ControllerClass),
													.pFnClassInit		= UI_ControllerClass_init_,
													.instanceSize		= sizeof(UI_Controller),
													.pFnInstanceInit	= UI_Controller_init_ },
							 GBL_TYPE_FLAG_TYPEINFO_STATIC);
	}

	return type;
}
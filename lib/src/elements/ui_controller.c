#include <ui/elements/ui_controller.h>
#include <ui/elements/ui_container.h>
#include <ui/elements/ui_root.h>


#include <gimbal/gimbal_containers.h>

#include <raylib.h>
#include <raymath.h>

static GblObject *GblObject_findDescendantByType(GblObject *pSelf, GblType descendantType) {
    if (!pSelf) return nullptr;

    GblObject *result = nullptr;
    GblArrayDeque queue;
    GblArrayDeque_construct(&queue, sizeof(GblObject*), 8);
    GblArrayDeque_pushBack(&queue, &pSelf);

    while (GblArrayDeque_size(&queue)) {
        GblObject **ppObj = GblArrayDeque_popFront(&queue);
        size_t childCount = GblObject_childCount(*ppObj);

        for (size_t i = 0; i < childCount; i++) {
            GblObject *childObj = GblObject_findChildByIndex(*ppObj, i);
            if (GBL_TYPEOF(childObj) == descendantType) {
                result = childObj;
                goto done;
            }
            GblArrayDeque_pushBack(&queue, &childObj);
        }
    }

    done:
    GblArrayDeque_destruct(&queue);
    return result;
}

static UI_Button *findDefaultSelectableInContainer_(UI_Container *pContainer) {
    UI_Button *pButton = GBL_AS(UI_Button, GblObject_findDescendantByType(GBL_OBJECT(pContainer), UI_BUTTON_TYPE));
    while (pButton) {
        if (pButton->isSelectedByDefault) return pButton;
        pButton = GBL_AS(UI_Button, GblObject_siblingNextByType(GBL_OBJECT(pButton), UI_BUTTON_TYPE));
    }
    return nullptr;
}

static GBL_RESULT UI_Controller_init_(GblInstance *pInstance) {
    UI_Controller *pController = UI_CONTROLLER(pInstance);
    UI_Widget     *pWidget     = UI_WIDGET(pInstance);

    pController->controllerId   = 0;
    pController->isKeyboard     = false;

	pWidget->r 		 = 0;
	pWidget->g 		 = 0;
	pWidget->b 		 = 0;
	pWidget->a 		 = 255;
	pWidget->z_index = 200;

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

GblType UI_Controller_type(void) {
	static GblType type = GBL_INVALID_TYPE;

	if (type == GBL_INVALID_TYPE) {
		type =
			GblType_register(GblQuark_internStatic("UI_Controller"),
							 UI_WIDGET_TYPE,
							 &(static GblTypeInfo){.classSize	    = sizeof(UI_ControllerClass),
												   .pFnClassInit    = UI_ControllerClass_init_,
												   .instanceSize    = sizeof(UI_Controller),
												   .pFnInstanceInit = UI_Controller_init_},
							 GBL_TYPE_FLAG_TYPEINFO_STATIC);
	}

	return type;
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
        if (GblObject_findChildByType(sibling, UI_BUTTON_TYPE)) return sibling;
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

static Vector2 vector2_FindClosestPoint_(Vector2 startCenter, Rectangle targetRect) {
    float min_x = targetRect.x;
    float max_x = targetRect.x + targetRect.width;
    float min_y = targetRect.y;
    float max_y = targetRect.y + targetRect.height;

    Vector2 closestPoint = {
        .x = GBL_CLAMP(startCenter.x, min_x, max_x),
        .y = GBL_CLAMP(startCenter.y, min_y, max_y)
    };

    return closestPoint;
}

static UI_Button *findSelectableByPosition_(UI_Button* pCurrent, UI_CONTROLLER_BUTTON direction) {
    UI_Root *pRoot = GBL_REQUIRE(UI_Root, "UI_Root");
    if GBL_UNLIKELY(!pRoot) return nullptr;

    Vector2 currPos    =   UI_get_absolute_position_(UI_WIDGET(pCurrent));
    Vector2 currSize   =  {UI_WIDGET(pCurrent)->w, UI_WIDGET(pCurrent)->h};
    Vector2 currCenter = {
        currPos.x + currSize.x * 0.5f,
        currPos.y + currSize.y * 0.5f
    };

    UI_Button* pBest      = nullptr;
    float      bestScore  = -FLT_MAX;

    // Iterate over all widgets in draw order
    GblArrayList *drawQueue = UI_drawQueue_get();
    for (size_t i = 0; i < GblArrayList_size(drawQueue); ++i) {
        GblObject **ppObj      = GblArrayList_at(drawQueue, i);
        UI_Button  *pCandidate = GBL_AS(UI_Button, *ppObj);

        if (!pCandidate               ||
             pCandidate == pCurrent   ||
            !pCandidate->isSelectable) {
            continue;
        }

        Vector2 candPos          =   UI_get_absolute_position_(UI_WIDGET(pCandidate));
        Vector2 candSize         =  {UI_WIDGET(pCandidate)->w, UI_WIDGET(pCandidate)->h};
        Vector2 candClosestPoint =   vector2_FindClosestPoint_(currCenter, (Rectangle){candPos.x, candPos.y,
                                                                                       candSize.x, candSize.y});

        Vector2 cursorDir        = {0,0};
        Vector2 delta            = Vector2Subtract(candClosestPoint, currCenter);
        float   dist             = Vector2Distance(candClosestPoint, currCenter);
        float   angle            = Vector2Angle(cursorDir, delta);

        switch (direction) {
            case UI_CONTROLLER_UP:
                if (delta.y >= 0) continue; // must be above
                cursorDir = (Vector2){0, -1};
                break;
            case UI_CONTROLLER_DOWN:
                if (delta.y <= 0) continue; // must be below
                cursorDir = (Vector2){0, 1};
                break;
            case UI_CONTROLLER_LEFT:
                if (delta.x >= 0) continue; // must be left
                cursorDir = (Vector2){-1, 0};
                break;
            case UI_CONTROLLER_RIGHT:
                if (delta.x <= 0) continue; // must be right
                cursorDir = (Vector2){1, 0};
                break;
            default:
                continue;
        }


        if(fabsf(angle) > 0.4f) continue;

        float score = 10.0f - dist;

        if (score > bestScore) {
            bestScore = score;
            pBest = pCandidate;
        }
    }

    return pBest;
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

    // If axis matches grandparent's orientation, find sibling container
    if (axis == grand_parent_orientation) {
        pNewContainer = findSiblingContainerWithButton_(pParent, next);
    }

    // Fallback to searching by position
    if (!pNewContainer) {
        return findSelectableByPosition_(UI_BUTTON(pSelf), buttonPress);
    }

    if (!pNewContainer) return nullptr;

    char new_orientation  = tolower(GBL_AS(UI_Container, pNewContainer)->orientation);
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

        if (!(*ppButton)) {
            newButton = GBL_AS(UI_Button, GblObject_findDescendantByType(GBL_OBJECT(root), UI_BUTTON_TYPE));
            if (!newButton) return GBL_RESULT_ERROR;
            *ppButton = newButton;
            (*ppButton)->isSelected = true;
            return GBL_RESULT_SUCCESS;
        }

        newButton = moveCursor_(GBL_OBJECT(*ppButton), eventButton);

        if (newButton) {
            (*ppButton)->isSelected = false;
            *ppButton = newButton;
            (*ppButton)->isSelected = true;
        }
    }

    if (*ppButton && isPress && !isDirection) {
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

void UI_Controller_sendButton(UI_Controller *pSelf, UI_CONTROLLER_BUTTON_STATE state, UI_CONTROLLER_BUTTON button) {
    UI_Controller_handleButton_(pSelf, state, button);
}

void UI_Controller_setSelectedButton(UI_Controller *pSelf, UI_Button* pButton) {
	UI_CONTROLLER(pSelf)->pSelectedButton = pButton;
}

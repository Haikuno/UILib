#ifndef UI_INTERNAL_H
#define UI_INTERNAL_H

#include <gimbal/gimbal_containers.h>
#include <gimbal/gimbal_meta.h>

void UI_drawQueue_init(void);
void UI_drawQueue_push(GblObject *pObj);
void UI_drawQueue_remove(GblObject *pObj);

GblArrayList *UI_getDrawQueue(void);

#endif
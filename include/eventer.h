#ifndef __EVENTER_H__
#define __EVENTER_H__


#include "entity.h"
#include "gfc_vector.h"


Entity* eventer_entity_new(GFC_Vector2D position, GFC_Vector2D size, Sprite* sprite, void* ontouch);

typedef struct EventerData_S {
	Uint8 num;
	void (*ontouch)();
}EventerData;

Entity* quester_entity_new(GFC_Vector2D position, int quest);

#endif
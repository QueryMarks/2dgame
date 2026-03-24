#ifndef __COLLIDER_H__
#define __COLLIDER_H__

#include "gfc_types.h"
#include "gfc_shape.h"
#include "entity.h"
/**
* @purpose this should allow us to create colliders and a collision system
**/

typedef struct Collider_S
{
	Uint8	_inuse;
	Entity*	entity; /**<the entity to which the collider belongs, null if no entity*/
	//GFC_Vector2D position; /**<the position of the collider, should be synced with the entity/tile*/
	GFC_Rect rect; /**the rectangle of the collider*/
	Bool isDynamic; /**true if the collider is dynamic*/


	void (*free)(struct Collider_S* self);

}Collider;

Collider* collider_new(GFC_Rect rect, Bool isDynamic, Entity* entity);

void collider_manager_init(Uint32 max, Bool isDynamic);
void collider_manager_close();
void collider_manager_check_collisions();
Uint8(checkCollision)(struct Collider_S* self, struct Collider_S* other);


#endif
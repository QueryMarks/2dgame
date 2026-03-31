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
	Uint8 team; /**<0 for solid, 1 for player-aligned, 2 for enemy- or otherwise damageable-aligned**/

	void (*free)(struct Collider_* self);

}Collider;

Collider* collider_new(GFC_Rect rect, Bool isDynamic, Entity* entity);

void collider_manager_init(Uint32 max, Bool isDynamic);
void collider_manager_close();
void collider_manager_draw_all();
void collider_free(Collider* self);
void collider_manager_check_collisions();
Uint8 checkCollision(struct Collider_S self, struct Collider_S other);

enum TEAM
{
	SOLID,
	PLAYER,
	ENEMY
};

GFC_Rect collider_manager_check_static_collisions(GFC_Rect rect);
#endif
#ifndef __SHOT_H__
#define __SHOT_H__

#include "entity.h"
#include "gfc_vector.h"

Entity* shot_entity_new(GFC_Vector2D position, GFC_Vector2D direction, float damage, int type, int timer);

typedef struct Shot_Data_S 
{
	Uint8		type; /**<the type of shot. 0 is normal, 1 is melee, 2 is explosive*/
	int			timer; /**<how long the shot has to live, in frames*/
	float		damage; /**<how much damage the shot does*/

}Shot_Data;

enum SHOTTYPE {
	PROJECTILE,
	MELEE,
	EXPLOSIVE,
	HITSCAN
};

#endif
#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "entity.h"
#include "gfc_vector.h"

Entity* enemy_entity_new(GFC_Vector2D position);
Entity* floater_enemy_entity_new(GFC_Vector2D position);
Entity* turret_enemy_entity_new(GFC_Vector2D position);
Entity* walky_enemy_entity_new(GFC_Vector2D position);
Entity* bruiser_enemy_entity_new(GFC_Vector2D position);


typedef struct Enemy_Data_S {
	float damage;
	int timer;

}Enemy_Data;

#endif
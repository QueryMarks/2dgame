#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "entity.h"
#include "gfc_vector.h"

static Entity *player = { 0 };

Entity *player_get();

Entity *player_entity_new(GFC_Vector2D position);

#endif
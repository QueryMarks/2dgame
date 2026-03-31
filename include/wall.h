#ifndef __WALL_H__
#define __WALL_H__

#include "entity.h"
#include "gfc_vector.h"

Entity* wall_entity_new(GFC_Vector2D position, int damagetype, GFC_Vector2D size);


#endif
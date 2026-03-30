#ifndef __SHOT_H__
#define __SHOT_H__

#include "entity.h"
#include "gfc_vector.h"

Entity* shot_entity_new(GFC_Vector2D position, GFC_Vector2D direction);

#endif
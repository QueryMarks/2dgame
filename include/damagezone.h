#ifndef __DAMAGEZONE_H__
#define __DAMAGEZONE_H__


#include "entity.h"
#include "gfc_vector.h"

Entity* damagezone_entity_new(GFC_Vector2D position, float damage, GFC_Vector2D size);

#endif
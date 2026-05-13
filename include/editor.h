#ifndef __EDITOR_H__
#define __EDITOR_H__


#include "simple_json.h"
#include "simple_logger.h"

#include "gfc_types.h"
#include "gfc_list.h"
#include "element.h"
#include "level.h"

typedef struct Editor_S {
	Uint8 entity_index;
	Uint8 tile_index;
	Uint8 entities_max;
	Uint8 tiles_max;
	GFC_List* sprite_array;
	Element* entity_button;
	Element* tile_button;
	Level* level;
}Editor;

void editor_init();
void editor_change_entity();
void editor_change_tile();
void editor_place_entity(GFC_Vector2D position);
void editor_place_tile(int tile_x, int tile_y);

#endif
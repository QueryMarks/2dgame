#include "simple_logger.h"

#include "player.h"


void player_think(Entity *self);

void player_update(Entity *self);

void player_free(Entity *self);

Entity *player_entity_new(GFC_Vector2D position)
{
	Entity *self;
	self = entity_new();
	if (!self) {
		slog("Failed to create player entity");
		return NULL;
	}
	self->sprite = gf2d_sprite_load_all(
		"images/ed210_top.png",
		128,
		128,
		16,
		0
	);
	self->position = position;
	self->frame = 0;
	self->scale = gfc_vector2d(1,1);
	
}

void player_think(Entity *self) {
	if (!self) return;
}

void player_update(Entity* self) {
	if (!self) return;
}

void player_free(Entity *self) {
	if (!self) return;
}

/*eol@eof*/
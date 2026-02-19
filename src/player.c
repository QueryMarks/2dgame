#include "simple_logger.h"
#include "gfc_input.h"


#include "player.h"


void player_think(Entity *self);

void player_update(Entity *self);

void player_free(Entity *self);

static Entity *player = { 0 };

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
	self->think = player_think;
	self->update = player_update;
	self->velocity = gfc_vector2d(0, 0);
	player = self;
	return self;
}

void player_think(Entity *self) {
	if (!self) return;
	if (gfc_input_key_down("d")) {
		slog("PRESSING RIGHT");
		self->velocity.x = 4;
	}
	else if (gfc_input_key_down("a")) {
		slog("PRESSING LEFT");
		self->velocity.x = -4;
	}
	else {
		self->velocity.x = 0;
	}

}

void player_update(Entity* self) {
	if (!self) return;
	self->position.x += self->velocity.x;
}

void player_free(Entity *self) {
	if (!self) return;
}

/*eol@eof*/
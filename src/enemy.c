#include "simple_logger.h"
#include "gfc_input.h"


#include "enemy.h"
#include "player.h"
#include "collider.h"


void enemy_think(Entity* self);

void enemy_update(Entity* self);

void enemy_free(Entity* self);


Entity* enemy_entity_new(GFC_Vector2D position)
{
	Entity* self;
	self = entity_new();
	if (!self) {
		slog("Failed to create enemy entity");
		return NULL;
	}
	self->sprite = gf2d_sprite_load_all(
		"images/space_bug_top.png",
		128,
		128,
		16,
		0
	);
	self->position = position;
	self->frame = 0;
	self->scale = gfc_vector2d(1, 1);
	self->think = enemy_think;
	self->update = enemy_update;
	self->velocity = gfc_vector2d(0, 0);
	self->collider = collider_new(gfc_rect(position.x, position.y, 128.0, 128.0), true, self);
	self->free = enemy_free;
	return self;
}

void enemy_think(Entity* self) {
	if (!self) return;
	Entity* my_player = player_get();
	if (!my_player) return;
	if (my_player->position.x > self->position.x) {
		self->velocity.x += .05;
	}
	else if (my_player->position.x < self->position.x) {
		self->velocity.x += -.05;
	}
/*	else {
		self->velocity.x = 0;
	}
*/

}

void enemy_update(Entity* self) {
	if (!self) return;
	self->position.x += self->velocity.x;
	if (self->collider) {
		Collider* collider = self->collider;
		if (collider->_inuse == 1) {
			collider->rect = gfc_rect(self->position.x, self->position.y, 128.0, 128.0);
		}
	}
}

void enemy_free(Entity* self) {
	if (!self) return;
}

/*eol@eof*/
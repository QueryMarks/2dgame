#include "simple_logger.h"
#include "gfc_input.h"


#include "player.h"
#include "camera.h"
#include "collider.h"


void player_think(Entity *self);

void player_update(Entity *self);

void player_free(Entity *self);

float gravity = 0.05;
float gravity_max = 4;

float move_accel = 0.3;
float move_friction = 0.1;
float move_water_friction = 0.01;
float move_max = 4;

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
	self->collider = collider_new(gfc_rect(position.x, position.y, 128.0, 128.0), true, self);
	self->free = player_free;
	player = self;
	//self->collider = collider_new(128, 128, true);
	return self;
}

void player_think(Entity *self) {
	if (!self) return;
	if (gfc_input_key_down("d")) {
		if (self->velocity.x + move_accel < move_max)
			self->velocity.x += move_accel;
		else if (self->velocity.x < move_max)
			self->velocity.x = move_max;
	}
	else if (gfc_input_key_down("a")) {
		if (self->velocity.x - move_accel > -move_max)
			self->velocity.x -= move_accel;
		else if (self->velocity.x > -move_max)
			self->velocity.x = -move_max;
	}
	else {
		//set to if false for now because we don't have the ground yet
		if (false) {
			if (self->velocity.x > 0) {
				if (self->velocity.x - move_friction < 0) {
					self->velocity.x = 0;
				}
				else
				{
					self->velocity.x -= move_friction;
				}
			}
			else if (self->velocity.x < 0) {
				if (self->velocity.x + move_friction > 0) {
					self->velocity.x = 0;
				}
				else
				{
					self->velocity.x += move_friction;
				}
			}
		}
		
	}
	if (gfc_input_key_down("l")) {
		self->velocity.y = -4;
	}
	else
	{
		if (self->velocity.y + gravity < gravity_max)
			self->velocity.y += gravity;
		else if (self->velocity.y < gravity_max)
			self->velocity.y = gravity_max;

	}

}

Entity* player_get() {
	if (!player) return NULL;
	return player;

}

void player_update(Entity* self) {
	if (!self) return;
	self->position.x += self->velocity.x;
	self->position.y += self->velocity.y;
	self->frame += 0.1;
	if (self->frame >= 16) {
		self->frame = 0;
	}
	if (self->collider) {
		Collider *collider = self->collider;
		if (collider->_inuse == 1) {
			collider->rect = gfc_rect(self->position.x, self->position.y, 128.0, 128.0);
		}
	}
	camera_center_on(self->position);
}

void player_free(Entity *self) {
	if (!self) return;
}

/*eol@eof*/
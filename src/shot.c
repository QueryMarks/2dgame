#include "simple_logger.h"


#include "collider.h"
#include "shot.h"

float shot_speed = 4;
GFC_Vector2D velocity;
void shot_think(Entity* self);

void shot_update(Entity* self);

void shot_free(Entity* self);

void shot_collide(Entity* self, void* collider);

Entity* shot_entity_new(GFC_Vector2D position, GFC_Vector2D direction) {
	Entity* self;
	self = entity_new();
	if (!self)return NULL;
	self->sprite = gf2d_sprite_load_all(
		"images/shot.png",
		32,
		32,
		1,
		0
	);
	self->position = position;
	self->frame = 0;
	self->scale = gfc_vector2d(1, 1);
	self->think = shot_think;
	self->update = shot_update;
	gfc_vector2d_scale(self->velocity, direction, 4);
	self->collider = collider_new(gfc_rect(position.x, position.y, 32, 32), true, self);
	self->collide = shot_collide;
	self->free = shot_free;
	Collider* collider = self->collider;
	collider->team = 1;
	return self;
}

void shot_think(Entity* self) {
	if (!self)return;

}

void shot_update(Entity* self) {
	slog("started shot update");
	if (!self)return;
	gfc_vector2d_add(self->position, self->position, self->velocity);
	Collider* collider = self->collider;
	collider->rect.x = self->position.x;
	collider->rect.y = self->position.y;
}

void shot_free(Entity*self) {
	if (!self)return;
}

void shot_collide(Entity* self, void*collider) {
	if (!collider) {
		return;
	}
	if (!self) return;
	Collider* other = collider;
	if (!other->isDynamic) {
		self->removeme = 1;
	}
	else if (other->team != 1) {
		self->removeme = 1;
	}
}

/*eol@eof*/
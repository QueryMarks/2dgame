#include "simple_logger.h"

#include "collider.h"
#include "shot.h"
#include "wall.h"

void wall_think(Entity* self);
void wall_update(Entity* self);
void wall_collide(Entity* self, void* collider);
void wall_free(Entity* self);

Entity* wall_entity_new(GFC_Vector2D position, int damagetype, GFC_Vector2D size)
{
	Entity* self;
	self = entity_new();
	if (!self) {
		slog("failed to create wall");
		return NULL;
	}
	if (damagetype == EXPLOSIVE) {
		self->sprite = gf2d_sprite_load_all(
			"images/wall_explosive.png",
			64,
			128,
			1,
			0
		);
	}
	else if (damagetype == MELEE) {
		self->sprite = gf2d_sprite_load_all(
			"images/wall_melee.png",
			64,
			128,
			1,
			0
		);
	}

	self->position = position;
	self->frame = 0;
	self->scale = gfc_vector2d(size.x/64.0f, size.y/128.0f);
	self->collider = collider_new(gfc_rect(position.x, position.y, size.x, size.y), true, self);
	self->collide = wall_collide;
	self->update = wall_update;
	self->think = wall_think;
	Collider* collider = self->collider;
	collider->team = ENEMY;
	self->damager = 0;
	self->free = wall_free;
	self->health = 1.0f;
	int* damage_type = malloc(sizeof(int));
	damage_type = damagetype;
	self->data = damage_type;
	self->solid = 1;
}

void wall_think() {
}

void wall_update() {
}

void wall_collide(Entity* self, void*collider)
{
	if (!self)return;
	if (!collider)return;
	Collider* collided = collider;
	if (collided->team == PLAYER) {
		if (collided->entity->damager == 1) {
			Shot_Data* damager = collided->entity->data;
			int my_type = (int)self->data;
			if (damager->type == my_type) {
				slog("I'VE BEEN HIT!");
				self->removeme = 1;
			}
		}
	}
}

void wall_free(Entity* self)
{
	if (!self)return;
}
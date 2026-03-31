#include "simple_logger.h"

#include "collider.h"
#include "shot.h"
#include "spike.h"
#include "enemy.h"

void spike_think(Entity* self);
void spike_update(Entity* self);
void spike_collide(Entity* self, void* collider);
void spike_free(Entity* self);

Entity* spike_entity_new(GFC_Vector2D position, float damage, GFC_Vector2D size)
{
	Entity* self;
	self = entity_new();
	if (!self) {
		slog("failed to create spike");
		return NULL;
	}
	self->sprite = gf2d_sprite_load_all(
		"images/spike.png",
		64,
		64,
		1,
		0
	);
	self->position = position;
	self->frame = 0;
	self->scale = gfc_vector2d(size.x / 64.0f, size.y / 64.0f);
	self->collider = collider_new(gfc_rect(position.x, position.y, size.x, size.y), true, self);
	self->collide = spike_collide;
	self->update = spike_update;
	self->think = spike_think;
	Collider* collider = self->collider;
	collider->team = ENEMY;
	self->damager = 2;
	self->free = spike_free;
	self->health = 1.0f;
	Enemy_Data* enemy_data = malloc(sizeof(Enemy_Data)); //used for damage etc
	if (enemy_data)
	{
		enemy_data->timer = 0;
		enemy_data->damage = 10.0f;
		self->data = enemy_data;
	}

	self->data = enemy_data;
	self->solid = 1;
	self->removeme = 0;
	slog("finished becoming spike");
}

void spike_think(Entity* self) {
}

void spike_update(Entity* self) {
}

void spike_collide(Entity* self, void* collider)
{
	if (!self)return;
	if (!collider)return;
	Collider* collided = collider;
	if (collided->isDynamic)
	{

	}
}

void spike_free(Entity* self)
{
	if (!self)return;
}
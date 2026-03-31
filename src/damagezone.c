#include "simple_logger.h"

#include "collider.h"
#include "shot.h"
#include "damagezone.h"
#include "enemy.h"

void damagezone_think(Entity* self);
void damagezone_update(Entity* self);
void damagezone_collide(Entity* self, void* collider);
void damagezone_free(Entity* self);

Entity* damagezone_entity_new(GFC_Vector2D position, float damage, GFC_Vector2D size)
{
	Entity* self;
	self = entity_new();
	if (!self) {
		slog("failed to create damagezone");
		return NULL;
	}
	self->sprite = gf2d_sprite_load_all(
		"images/damagezone.png",
		64,
		64,
		1,
		0
	);
	self->position = position;
	self->frame = 0;
	self->scale = gfc_vector2d(size.x / 64.0f, size.y / 64.0f);
	self->collider = collider_new(gfc_rect(position.x, position.y, size.x, size.y), true, self);
	self->collide = damagezone_collide;
	self->update = damagezone_update;
	self->think = damagezone_think;
	Collider* collider = self->collider;
	collider->team = ENEMY;
	self->damager = 0;
	self->free = damagezone_free;
	self->health = 1.0f;
	float* my_damage = malloc(sizeof(float)); //used for damage etc
	if (my_damage)
	{
		my_damage = &damage;
		self->data = my_damage;
	}

	self->data = my_damage;
	self->solid = 0;
	self->removeme = 0;
	slog("finished becoming damagezone");
}

void damagezone_think(Entity* self) {
}

void damagezone_update(Entity* self) {
}

void damagezone_collide(Entity* self, void* collider)
{
	if (!self)return;
	if (!collider)return;
	Collider* collided = collider;
	if (collided->isDynamic)
	{
		if (collided->team == PLAYER)
		{
			float* my_damage = self->data;
			float damage = *my_damage;
			collided->entity->health -= 0.1f;
		}
	}
}

void damagezone_free(Entity* self)
{
	if (!self)return;
}
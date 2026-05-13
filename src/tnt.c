#include "simple_logger.h"

#include "collider.h"
#include "shot.h"
#include "tnt.h"

void tnt_think(Entity* self);
void tnt_update(Entity* self);
void tnt_collide(Entity* self, void* collider);
void tnt_free(Entity* self);

Entity* tnt_entity_new(GFC_Vector2D position)
{
	Entity* self;
	self = entity_new();
	if (!self) {
		slog("failed to create tnt");
		return NULL;
	}
	self->entity_type = "tnt";
	self->sprite = gf2d_sprite_load_all(
		"images/tnt.png",
		64,
		64,
		1,
		0
	);
	self->position = position;
	self->frame = 0;
	self->scale = gfc_vector2d(1.0f, 1.0f);
	self->collider = collider_new(gfc_rect(position.x, position.y, 64.0f, 64.0f), true, self);
	self->collide = tnt_collide;
	self->update = tnt_update;
	self->think = tnt_think;
	Collider* collider = self->collider;
	collider->team = ENEMY;
	self->damager = 0;
	self->free = tnt_free;
	self->health = 1.0f;
	self->solid = 1;
	self->removeme = 0;
	int* timer = malloc(sizeof(int));
	timer = -1;
	self->data = timer;
}

void tnt_think(Entity* self) {
	
	int* my_timer = self->data;
	int timer = (int)my_timer;
	if ((int)timer == 0)
	{
		Entity* shot = shot_entity_new(self->position, gfc_vector2d(0, 0), 50.0f, MELEE, 5);
		shot->sprite = gf2d_sprite_load_all(
			"images/explosion.png",
			64,
			64,
			1,
			0
		);
		Collider* shot_collider = shot->collider;
		shot_collider->team = ENEMY;
		shot_collider->rect.h = 64;
		shot_collider->rect.w = 64;
		self->removeme = 1;
	}
	else if (timer > -1) {
		timer -= 1;
	}
	self->data = timer;
}

void tnt_update(Entity* self) {
}

void tnt_collide(Entity* self, void* collider)
{
	if (!self)return;
	if (!collider)return;
	Collider* collided = collider;
	if (collided->isDynamic)
	{
		if (collided->team == PLAYER) {
			int* timer = self->data;
			if (timer == -1){
				timer = 60;
				self->data = timer;
				self->frame = 1;
			}
		}
	}
}

void tnt_free(Entity* self)
{
	if (!self)return;
}
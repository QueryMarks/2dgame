#include "simple_logger.h"

#include "collider.h"
#include "shot.h"
#include "current.h"

void current_think(Entity* self);
void current_update(Entity* self);
void current_collide(Entity* self, void* collider);
void current_free(Entity* self);

Entity* current_entity_new(GFC_Vector2D position, GFC_Vector2D direction, GFC_Vector2D size)
{
	Entity* self;
	self = entity_new();
	if (!self) {
		slog("failed to create current");
		return NULL;
	}
	self->entity_type = "current";
	self->sprite = gf2d_sprite_load_all(
		"images/current.png",
		64,
		128,
		1,
		0
	);
	self->position = position;
	self->frame = 0;
	self->scale = gfc_vector2d(size.x / 64.0f, size.y / 128.0f);
	self->collider = collider_new(gfc_rect(position.x, position.y, size.x, size.y), true, self);
	self->collide = current_collide;
	self->update = current_update;
	self->think = current_think;
	Collider* collider = self->collider;
	collider->team = ENEMY;
	self->damager = 0;
	self->free = current_free;
	self->health = 1.0f;
	GFC_Vector2D* current_dir = malloc(sizeof(GFC_Vector2D));
	if (current_dir)
	{
		current_dir->x = direction.x;
		current_dir->y = direction.y;
	}
	self->data = current_dir;
	self->solid = 0;
	self->removeme = 0;
}

void current_think(Entity*self) {
}

void current_update(Entity*self) {
}

void current_collide(Entity* self, void* collider)
{
	if (!self)return;
	if (!collider)return;
	Collider* collided = collider;
	if (collided->isDynamic)
	{
		GFC_Vector2D currentdir;
		currentdir = *(GFC_Vector2D*)self->data;
		collided->entity->velocity.x += currentdir.x;
		collided->entity->velocity.y += currentdir.y;
	}
}

void current_free(Entity* self)
{
	if (!self)return;
}
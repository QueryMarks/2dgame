#include "simple_logger.h"


#include "collider.h"
#include "shot.h"

float shot_speed = 4;
GFC_Vector2D velocity;

void shot_think(Entity* self);

void shot_update(Entity* self);

void shot_free(Entity* self);

void shot_collide(Entity* self, void* collider);



Entity* shot_entity_new(GFC_Vector2D position, GFC_Vector2D direction, float damage, int type, int timer) {
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
	self->damager = 1;
	Collider* collider = self->collider;
	collider->team = 1;
	//data is used for the timer
	//self->data = malloc(sizeof(Shot_Data));
	Shot_Data *shot_data = malloc(sizeof(Shot_Data));
	if (shot_data) {
		shot_data->timer = timer;
		shot_data->type = type;
		shot_data->damage = damage;
		self->data = shot_data;
	}



	return self;
	slog("I LIVE");
}

void shot_think(Entity* self) {
	if (!self)return;
	Shot_Data* shot_data = self->data;
	if (shot_data->type == EXPLOSIVE) {
		self->velocity.y += 0.05;
	}

}

void shot_update(Entity* self) {
	if (!self)return;
	gfc_vector2d_add(self->position, self->position, self->velocity);
	Collider* collider = self->collider;
	collider->rect.x = self->position.x;
	collider->rect.y = self->position.y;
	
	Shot_Data* shot_data = self->data;
	int timer = shot_data->timer;
	//slog("MY TIMER IS %d", timer);
	if (timer == 0) {
		self->removeme = 1;
	}
	else if (timer > 0) {
		shot_data->timer -= 1;
	}
}

void shot_free(Entity*self) {
	if (!self)return;
	//free(self->data);
}

void shot_explode(Entity* self) {
	GFC_Vector2D shotpoint;
	gfc_vector2d_add(shotpoint, self->position, gfc_vector2d(-16*5 + (self->facing * 16*5), -16*5));
	//gfc_vector2d_add(shotpoint, shotpoint, gfc_vector2d(-16, -16));
	Entity* shot = shot_entity_new(shotpoint, gfc_vector2d(0, 0), 1, HITSCAN, 10);
	shot->scale = gfc_vector2d(5, 5);
	Collider* collider = shot->collider;
	collider->rect.h *= 5;
	collider->rect.w *= 5;
}
void shot_collide(Entity* self, void*collider) {
	if (!collider) {
		return;
	}
	if (!self) return;
	Collider* other = collider;
	Collider* mine = self->collider;
	Shot_Data* shot_data = self->data;
	if (!other->isDynamic) {
		if (shot_data->type == 0)
		{
			self->removeme = 1;
		}
		else if (shot_data->type == EXPLOSIVE) {
			shot_explode(self);
			self->removeme = 1;
		}
	}
	else if (other->team != mine->team) {
		if (shot_data->type != HITSCAN)
		{
			self->removeme = 1;
		}
		if (shot_data->type == EXPLOSIVE) {
			shot_explode(self);
		}
	}
}


/*eol@eof*/
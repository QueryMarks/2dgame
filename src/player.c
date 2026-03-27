#include "simple_logger.h"
#include "gfc_input.h"


#include "player.h"
#include "camera.h"
#include "collider.h"
#include "gf2d_draw.h"


void player_think(Entity *self);

void player_update(Entity *self);

void player_free(Entity *self);

void player_collide(void* collider);

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
	self->collider = collider_new(gfc_rect(position.x, position.y, 64, 64), true, self);
	self->free = player_free;
	self->collide = player_collide;
	player = self;
	//self->collider = collider_new(128, 128, true);
	return self;
}

GFC_Vector2D player_check_move(Entity* self, GFC_Vector2D move_position) {
	if (self->collider) {
		Collider* collider = self->collider;
		if (collider->_inuse == 1) {
			GFC_Rect newRect = collider->rect;
			newRect.x = move_position.x;
			newRect.y = move_position.y;
			GFC_Rect found_collider = collider_manager_check_static_collisions(newRect);
			int adjusted = 0;
			//if (!(found_collider.h == 0 && found_collider.w == 0))
			while (!(found_collider.h == 0 && found_collider.w == 0) && adjusted <= 10000)
			{

				//add the code to move the character to the edge of the collider here
				GFC_Vector2D my_center = gfc_rect_get_center_point(newRect);
				GFC_Vector2D found_center = gfc_rect_get_center_point(found_collider);
				GFC_Vector2D center_subtract;
				gfc_vector2d_add(center_subtract, my_center, -found_center);
				double vectorRight = -(newRect.x - (found_collider.x + found_collider.w));
				double vectorLeft = -((newRect.w + newRect.x) - found_collider.x);
				double vectorUp = -((newRect.h + newRect.y)- found_collider.y);
				double vectorDown = -(newRect.y - (found_collider.y + found_collider.h));
				/*
				if (vectorRight == 0) {
					vectorRight = 1000;
				}
				if (vectorLeft == 0) {
					vectorLeft = 1000;
				}
				if (vectorUp == 0) {
					vectorUp = 1000;
				}
				if (vectorDown == 0) {
					vectorDown = 1000;
				}

				if ((vectorLeft == 1000) && (vectorRight == 1000) && (vectorUp == 1000) && (vectorDown == 1000)) {
					//somehow we would become exactly aligned with a tile. just don't move
					return player->position;
				}*/
				if (abs(abs(center_subtract.x) - abs(center_subtract.y)) <= 1) {
					if (abs(vectorRight) < abs(vectorLeft)) {
						move_position.x += vectorRight;
					}
					else {
						move_position.x += vectorLeft;
					}
					if (abs(vectorUp) < abs(vectorDown)) {
						move_position.y += vectorUp;
					}
					else {
						move_position.y += vectorDown;
					}
				}
				else
				{
					if (abs(center_subtract.x) > abs(center_subtract.y))//(min(abs(vectorRight), abs(vectorLeft)) < min(abs(vectorUp), abs(vectorDown))) 
					{
						if (abs(vectorRight) < abs(vectorLeft)) {
							move_position.x += vectorRight;
						}
						else {
							move_position.x += vectorLeft;
						}
						self->velocity.x = 0;
					}
					else
					{
						if (abs(vectorUp) < abs(vectorDown)) {
							move_position.y += vectorUp;
						}
						else {
							move_position.y += vectorDown;
						}
						self->velocity.y = 0;
					}
					//player_check_move(self, move_position);
					//move_position = self->position;//player_check_move(self, move_position);
					
				}
				newRect.y = move_position.y;
				newRect.x = move_position.x;
				found_collider = collider_manager_check_static_collisions(newRect);
				adjusted += 1;

			}
		}
	}
	return move_position;
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
	GFC_Vector2D new_position = gfc_vector2d(self->position.x + self->velocity.x, self->position.y + self->velocity.y);
	if (self->collider) {
		Collider* collider = self->collider;
		
		new_position = player_check_move(self, new_position);
	}
	//self->position.x += self->velocity.x;
	//self->position.y += self->velocity.y;
	self->position = new_position;
	self->frame += 0.1;
	if (self->frame >= 16) {
		self->frame = 0;
	}
	if (self->collider) {
		Collider *collider = self->collider;
		if (collider->_inuse == 1) {
			collider->rect = gfc_rect(self->position.x, self->position.y, 64, 64);
			gf2d_draw_rect(collider->rect, gfc_color(1, 0, 1, 1));
		}
	}
	camera_center_on(self->position);
}

void player_free(Entity *self) {
	if (!self) return;
}

void player_collide(void* collider) {
	Collider *other = collider;
	if (!other->isDynamic) {
		slog("touching static collider in player");
	}
}

/*eol@eof*/
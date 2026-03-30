#include "simple_logger.h"
#include "gfc_input.h"


#include "enemy.h"
#include "player.h"
#include "collider.h"


void enemy_think(Entity* self);

void enemy_update(Entity* self);

void enemy_free(Entity* self);

void enemy_collide(Entity* self, void* collider);


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
	self->collide = enemy_collide;
	Collider* collider = self->collider;
	collider->team = 2;
	self->free = enemy_free;
	//self->collide = enemy_collide;
	return self;
}

GFC_Vector2D enemy_check_move(Entity* self, GFC_Vector2D move_position) {
	if (self->collider) {
		Collider* collider = self->collider;
		if (collider->_inuse == 1) {
			GFC_Rect newRect = collider->rect;
			newRect.x = move_position.x;
			newRect.y = move_position.y;
			GFC_Rect found_collider = collider_manager_check_static_collisions(newRect);
			int adjusted = 0;
			//if (!(found_collider.h == 0 && found_collider.w == 0))
			while (!(found_collider.h == 0 && found_collider.w == 0) && adjusted <= 10)
			{

				//add the code to move the character to the edge of the collider here
				GFC_Vector2D my_center = gfc_rect_get_center_point(newRect);
				GFC_Vector2D found_center = gfc_rect_get_center_point(found_collider);
				GFC_Vector2D center_subtract;
				gfc_vector2d_add(center_subtract, my_center, -found_center);
				double vectorRight = -(newRect.x - (found_collider.x + found_collider.w));
				double vectorLeft = -((newRect.w + newRect.x) - found_collider.x);
				double vectorUp = -((newRect.h + newRect.y) - found_collider.y);
				double vectorDown = -(newRect.y - (found_collider.y + found_collider.h));
				if ((abs(center_subtract.x) > abs(center_subtract.y)))
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
					//self->velocity.y = 0;
				}
				slog("the vectors are AS FOLLOWS: Left Right Up Down: %f, %f, %f, %f", vectorLeft, vectorRight, vectorUp, vectorDown);
				newRect.y = move_position.y;
				newRect.x = move_position.x;
				found_collider = collider_manager_check_static_collisions(newRect);
				adjusted += 1;

			}
		}
	}
	return move_position;
}

void enemy_think(Entity* self) {
	if (!self) {
		slog("enemies don't exist anymore");
		return;
	}
	Entity* my_player = player_get();
	if (!my_player) return;
	if (my_player->position.x > self->position.x) {
		self->velocity.x += .02;
	}
	else if (my_player->position.x < self->position.x) {
		self->velocity.x += -.02;
	}
/*	else {
		self->velocity.x = 0;
	}
*/

}

void enemy_update(Entity* self) {
	if (!self) {
		slog("enemies don't exist anymore");
		return;
	}
	GFC_Vector2D new_position = self->position;
	new_position.x += self->velocity.x;
	if (self->collider) {
		Collider* collider = self->collider;

		self->position = enemy_check_move(self, new_position);
	}
	
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

void enemy_collide(Entity* self, void* collider) {
	{
		if (!collider) return;
		Collider* collided = collider;
		if (collided->team == 1) {
			self->removeme = 1;
		}
	}
}
/*eol@eof*/
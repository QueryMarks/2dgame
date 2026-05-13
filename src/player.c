#include "simple_logger.h"
#include "gfc_input.h"


#include "player.h"
#include "camera.h"
#include "collider.h"
#include "gf2d_draw.h"
#include "shot.h"
#include "enemy.h"

#include "game.h"

void player_think(Entity *self);

void player_update(Entity *self);

void player_free(Entity *self);

void player_collide(Entity *self, void* collider);

float gravity = 0.1f;
float gravity_max = 4;

GFC_Rect ground_check_rect;

float move_accel = 0.1f;
float move_friction = 0.05f;
float move_water_friction = 0.01f;
float move_max = 3;

//bools for dashes etc
int candash = 1;
int float_timer = 60;

float buoy_accel = 0.1f;
int buoy_timer = 0;
int can_buoying = 0;

int weapon = 0;
int weaponmax = 9;

Entity *player_entity_new(GFC_Vector2D position)
{
	Entity *self;
	self = entity_new();
	if (!self) {
		slog("Failed to create player entity");
		return NULL;
	}
	self->entity_type = "player";
	self->sprite = gf2d_sprite_load_all(
		"images/player.png",
		64,
		64,
		1,
		0
	);
	self->position = position;
	self->frame = 0;
	self->scale = gfc_vector2d(1,1);
	self->think = player_think;
	self->update = player_update;
	self->velocity = gfc_vector2d(0, 0);
	self->collider = collider_new(gfc_rect(position.x, position.y, 64, 64), true, self);
	self->facing = 1;
	Collider* collider = self->collider;
	collider->team = 1;
	self->free = player_free;
	self->collide = player_collide;
	player = self;
	self->health = 100;
	self->iframes = 0;
	self->damager = 0;
	self->data = weapon;
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
			if (found_collider.h == 0) {
				found_collider = collider_manager_check_solid_collisions(newRect);
			}
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
				double vectorUp = -((newRect.h + newRect.y)- found_collider.y);
				double vectorDown = -(newRect.y - (found_collider.y + found_collider.h));
				if ((abs(min(abs(vectorRight), abs(vectorLeft)))) < (abs(min(abs(vectorUp), abs(vectorDown)))))
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
				//slog("the vectors are AS FOLLOWS: Left Right Up Down: %f, %f, %f, %f", vectorLeft, vectorRight, vectorUp, vectorDown);
				newRect.y = move_position.y;
				newRect.x = move_position.x;
				found_collider = collider_manager_check_static_collisions(newRect);
				if (found_collider.h == 0) {
					found_collider = collider_manager_check_solid_collisions(newRect);
				}
				adjusted += 1;

			}
		}
	}
	return move_position;
}


void player_think(Entity* self) {
	if (!self) return;

	if (collider_manager_check_static_collisions(ground_check_rect).h != 0)
	{
		candash = 1;
		can_buoying = 1;
		buoy_timer = 0;
		float_timer = 60;
	}
	if (gfc_input_key_down("d")) {
		self->facing = 1;
		if (self->velocity.x + move_accel < move_max)
			self->velocity.x += move_accel;
		else if (self->velocity.x < move_max)
			self->velocity.x = move_max;
	}
	else if (gfc_input_key_down("a")) {
		self->facing = -1;
		if (self->velocity.x - move_accel > -move_max)
			self->velocity.x -= move_accel;
		else if (self->velocity.x > -move_max)
			self->velocity.x = -move_max;
	}
	else {
		//set to if false for now because we don't have the ground yet
		if (collider_manager_check_static_collisions(ground_check_rect).h != 0) {
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
	if (gfc_input_key_pressed("l")) {
		if (collider_manager_check_static_collisions(ground_check_rect).h != 0) {
			self->velocity.y = -4;
		}
		else if (can_buoying == 1) {
			self->velocity.y = 0;
			buoy_timer = 60;
			can_buoying = 0;
		}
		
	}
	else
	{
		//float/gravity checking
		if (gfc_input_key_down("w") && float_timer > 0) {
			float_timer -= 1;
			self->velocity.y = 0.1;
		}
		else if (buoy_timer > 0) {
			buoy_timer -= 1;
			if (!(gfc_input_key_down("l")))
			{
				buoy_timer -= 1;
			}
			self->velocity.y -= buoy_accel;
			
		}
		else {
			float temp_gravity = gravity;
				if (gfc_input_key_down("l")) {
					temp_gravity *= 0.5;
				}
				if (self->velocity.y + temp_gravity < gravity_max)
					self->velocity.y += temp_gravity;
				else if (self->velocity.y < gravity_max)
					self->velocity.y = gravity_max;
		}

	}

	if (gfc_input_key_pressed("'")) {
		if (weapon < 6) {
			weapon += 1;
		}
		else {
			weapon = 0;
		}
	}
	//fire weapons here
	if (gfc_input_key_pressed(";")) {
		//move the weapon logic to different weapon files in the future.
		//BUSTER
		if (weapon == 0) {
			Collider* collider = self->collider;
			GFC_Vector2D shotpoint;
			gfc_vector2d_add(shotpoint, gfc_rect_get_center_point(collider->rect), gfc_vector2d(-16, -16));
			shot_entity_new(shotpoint, gfc_vector2d(self->facing, 0),5.0f,0,3000000);
		}
		//TRIPLET
		else if (weapon == 1)
		{
			Collider* collider = self->collider;
			GFC_Vector2D shotpoint;
			gfc_vector2d_add(shotpoint, gfc_rect_get_center_point(collider->rect), gfc_vector2d(-16, -16));
			shot_entity_new(shotpoint, gfc_vector2d(self->facing*3, 0), 5.0f, 0, 20);
			shot_entity_new(shotpoint, gfc_vector2d(self->facing*3, 0.3),5.0f, 0, 20);
			shot_entity_new(shotpoint, gfc_vector2d(self->facing*3, -0.3),5.0f, 0, 20);
		}
		//TWOSIDE
		else if (weapon == 2) {
			Collider* collider = self->collider;
			GFC_Vector2D shotpoint;
			gfc_vector2d_add(shotpoint, gfc_rect_get_center_point(collider->rect), gfc_vector2d(-16, -16));
			shot_entity_new(shotpoint, gfc_vector2d(self->facing * 0.5, 0), 5.0f, 0, 300);
			shot_entity_new(shotpoint, gfc_vector2d(self->facing * -0.5, 0), 5.0f, 0, 300);
		}
		//BIGSHOT
		else if (weapon == 3) {
			Collider* collider = self->collider;
			GFC_Vector2D shotpoint;
			gfc_vector2d_add(shotpoint, gfc_rect_get_center_point(collider->rect), gfc_vector2d(-32, -34));
			Entity* shot = shot_entity_new(shotpoint, gfc_vector2d(self->facing * 0.25, 0), 8.0f, 0, 120);
			shot->scale = gfc_vector2d(2, 2);
			Collider* shotcollider = shot->collider;
			shotcollider->rect.w *= 2;
			shotcollider->rect.h *= 2;
		}
		//SWORDITUDE
		else if (weapon == 4) {
			Collider* collider = self->collider;
			GFC_Vector2D shotpoint;
			gfc_vector2d_add(shotpoint, gfc_rect_get_center_point(collider->rect), gfc_vector2d(-32 + (self->facing * 48), -65));
			Entity* shot = shot_entity_new(shotpoint, gfc_vector2d(0, 0), 10.0f, MELEE, 3);
			shot->scale = gfc_vector2d(2, 3);
			Collider* shotcollider = shot->collider;
			shotcollider->rect.w *= 2;
			shotcollider->rect.h *= 3;

		}
		//SEALASER
		else if (weapon == 5) {
			Collider* collider = self->collider;
			GFC_Vector2D shotpoint;
			gfc_vector2d_add(shotpoint, gfc_rect_get_center_point(collider->rect), gfc_vector2d(-32 * 250 + (self->facing * 32 * 250), 0));
			Entity* shot = shot_entity_new(shotpoint, gfc_vector2d(0, 0), 4.0f, HITSCAN, 3);
			shot->scale = gfc_vector2d(500, 0.1);
			Collider* shotcollider = shot->collider;
			shotcollider->rect.w *= 500;
			shotcollider->rect.h *= 0.1;

		}
		//BUBBLE BOMB
		else if (weapon == 6) {
			Collider* collider = self->collider;
			GFC_Vector2D shotpoint;
			gfc_vector2d_add(shotpoint, gfc_rect_get_center_point(collider->rect), gfc_vector2d(-32 + (self->facing * 32), 0));
			Entity* shot = shot_entity_new(shotpoint, gfc_vector2d(self->facing, -0.25), 1.0f, EXPLOSIVE, 600);

		}

	}

	if (gfc_input_key_pressed("p") && candash == 1) {
		self->velocity.x = 8 * self->facing;
		candash = false;
	}
	self->data = weapon;
}

Entity* player_get() {
	if (!player) return NULL;
	return player;

}

void player_update(Entity* self) {
	if (!self) return;
	if (self->removeme == 1) {
		slog("I'M DEAD AS A DOORNAIL!!!");
	}
	GFC_Vector2D new_position = gfc_vector2d(self->position.x + self->velocity.x, self->position.y + self->velocity.y);
	if (self->collider) {

		new_position = player_check_move(self, new_position);
	}
	//self->position.x += self->velocity.x;
	//self->position.y += self->velocity.y;
	self->position = new_position;
	self->frame += 0.05;
	if (self->frame >= 3) {
		self->frame = 0;
	}
	if (self->collider) {
		Collider* collider = self->collider;
		if (collider->_inuse == 1) {
			collider->rect = gfc_rect(self->position.x, self->position.y, 64, 64);
			//gf2d_draw_rect(collider->rect, gfc_color(1, 0, 1, 1));
			ground_check_rect = gfc_rect(collider->rect.x, collider->rect.y + collider->rect.h, collider->rect.w, 2);
			//gf2d_draw_rect(ground_check_rect, gfc_color(0, 0, 1, 1));
		}
	}
	if (self->iframes > 0) {
		self->iframes -= 1;
	}
	camera_center_on(self->position);
}

void player_free(Entity* self) {
	if (!self) return;
}

void player_collide(Entity* self, void* collider) {
	if (!collider) return;
	Collider* other = collider;
	if (other->isDynamic) {
		if (other->team == ENEMY && other->entity->damager != 0 && self->iframes <= 0) {
			float damage = 0.0f;
			if (other->entity->damager == 2)
			{
				Enemy_Data *enemy_data = other->entity->data;
				damage = enemy_data->damage;
			}
			else if (other->entity->damager == 1)
			{
				Shot_Data *shot_data = other->entity->data;
				damage = shot_data->damage;
			}
			
			self->health -= damage;
			if (self->health <= 0){
				//return to the title screen
				game_state_change(GS_TITLE);
			}
			else {
				self->iframes = 60;
				int dir = 1;
				if ((self->position.x - other->entity->position.x) < 0) {
					dir = -1;
				}
				self->velocity = gfc_vector2d(dir * 3, -3);
			}
		}
	}
}
/*eol@eof*/
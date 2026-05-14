#include "simple_logger.h"
#include "gfc_input.h"


#include "enemy.h"
#include "player.h"
#include "collider.h"
#include "shot.h"
#include "quest.h"


void enemy_think(Entity* self);

void enemy_update(Entity* self);

void enemy_free(Entity* self);

void enemy_collide(Entity* self, void* collider);

//SWINGER
Entity* enemy_entity_new(GFC_Vector2D position)
{
	Entity* self;
	self = entity_new();
	if (!self) {
		slog("Failed to create enemy entity");
		return NULL;
	}
	self->entity_type = "swinger";
	self->sprite = gf2d_sprite_load_all(
		"images/swinger.png",
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
	collider->team = ENEMY;
	self->damager = 2;
	self->free = enemy_free;
	Enemy_Data *enemy_data = malloc(sizeof(Enemy_Data)); //used for damage etc
	if (enemy_data)
	{
		enemy_data->damage = 10.0f;
		self->data = enemy_data;
	}
	self->health = 20.0f;
	
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
					self->velocity.y = 0;
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
			if (collided->entity->damager == 1) {
				if (self->data == NULL) {
					return;
				}
				Shot_Data* damager = collided->entity->data;
				self->health -= damager->damage;
				
				if (self->health <= 0.0f) {
					audio_sfx_play("audio/sfx/enemyhurt2.wav");
					self->removeme = 1;
				}
				else
				{
					audio_sfx_play("audio/sfx/enemyhurt.wav");
				}

			}
		}
	}
}


//ENEMY 2: FLOATER
void floater_enemy_think(Entity* self);

void floater_enemy_update(Entity* self);

void floater_enemy_free(Entity* self);

void floater_enemy_collide(Entity* self, void* collider);


Entity* floater_enemy_entity_new(GFC_Vector2D position)
{
	Entity* self;
	self = entity_new();
	if (!self) {
		slog("Failed to create floater entity");
		return NULL;
	}
	self->entity_type = "floater";
	self->sprite = gf2d_sprite_load_all(
		"images/floater.png",
		64,
		64,
		1,
		0
	);
	self->position = position;
	self->frame = 0;
	self->scale = gfc_vector2d(1, 1);
	self->think = floater_enemy_think;
	self->update = floater_enemy_update;
	self->velocity = gfc_vector2d(0, 0);
	self->collider = collider_new(gfc_rect(position.x, position.y, 64.0, 64.0), true, self);
	self->collide = floater_enemy_collide;
	Collider* collider = self->collider;
	collider->team = ENEMY;
	self->damager = 2;
	self->free = enemy_free;
	self->health = 20.0f;
	Enemy_Data* enemy_data = malloc(sizeof(Enemy_Data)); //used for damage etc
	if (enemy_data)
	{
		enemy_data->timer = 0;
		enemy_data->damage = 10.0f;
		self->data = enemy_data;
	}

	self->data = enemy_data;
	//self->collide = enemy_collide;
	return self;
}

GFC_Vector2D floater_enemy_check_move(Entity* self, GFC_Vector2D move_position) {
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
				newRect.y = move_position.y;
				newRect.x = move_position.x;
				found_collider = collider_manager_check_static_collisions(newRect);
				adjusted += 1;

			}
		}
	}
	return move_position;
}

void floater_enemy_think(Entity* self) {
	if (!self) {
		slog("enemies don't exist anymore");
		return;
	}
	Enemy_Data* enemy_data = self->data;
	Entity* my_player = player_get();
	GFC_Vector2D difference;
	gfc_vector2d_add(difference, my_player->position, -self->position);
	if ((!my_player) || (gfc_vector2d_magnitude(difference) > 300)) {
		self->velocity.x = 0;
		if (self->velocity.y >= 3 && enemy_data->timer <= 0) {
			self->velocity.y = -3;
		}
		else {
			//gravity
			self->velocity.y += 0.025f;
		}
			
	}
	else
	{
		if (self->velocity.y >= 3 && enemy_data->timer <= 0) {
			self->velocity.y = (my_player->position.y - self->position.y) * 0.01f - 1.5f;
			self->velocity.x = (my_player->position.x - self->position.x) * 0.01f;
			enemy_data->timer = 30;
			slog("jumped");
		}
		else
		{
			if (enemy_data->timer <= 0)
			{
				if (my_player->position.x > self->position.x) {
					self->velocity.x = 1;
				}
				else if (my_player->position.x < self->position.x) {
					self->velocity.x = -1;
				}
			}
			//gravity
			self->velocity.y += 0.025f;
		}
	}

	if (enemy_data->timer > 0)
	{
		enemy_data->timer -= 1;
	}
	/*	else {
			self->velocity.x = 0;
		}
	*/

}

void floater_enemy_update(Entity* self) {
	if (!self) {
		slog("enemies don't exist anymore");
		return;
	}
	GFC_Vector2D new_position = self->position;
	new_position.x += self->velocity.x;
	new_position.y += self->velocity.y;
	if (self->collider) {
		Collider* collider = self->collider;

		self->position = floater_enemy_check_move(self, new_position);
	}

	if (self->collider) {
		Collider* collider = self->collider;
		if (collider->_inuse == 1) {
			collider->rect = gfc_rect(self->position.x, self->position.y, 64.0, 64.0);
		}
	}
}

void floater_enemy_free(Entity* self) {
	if (!self) return;
}

void floater_enemy_collide(Entity* self, void* collider) {
	{
		if (!collider) return;
		Collider* collided = collider;
		if (collided->team == 1) {
			if (collided->entity->damager == 1) {
				if (self->data == NULL) {
					return;
				}
				Shot_Data* damager = collided->entity->data;
				self->health -= damager->damage;

				if (self->health <= 0.0f) {
					if (get_quest_progress(QUEST_FLOATER) == 1)
					{
						slog("floater quest");
						set_quest_progress(QUEST_FLOATER, 2);
					}
					self->removeme = 1;
					audio_sfx_play("audio/sfx/enemyhurt2.wav");
				}
				else
				{
					audio_sfx_play("audio/sfx/enemyhurt.wav");
					int dir = 1;
					if ((self->position.x - collided->entity->position.x) < 0) {
						dir = -1;
					}
					self->velocity = gfc_vector2d(dir * damager->damage * 0.5, -damager->damage * 0.5);
				}

			}
		}
	}
}


///////////////
//TURRET///////
///////////////
void turret_enemy_think(Entity* self);

void turret_enemy_update(Entity* self);

void turret_enemy_free(Entity* self);

void turret_enemy_collide(Entity* self, void* collider);


Entity* turret_enemy_entity_new(GFC_Vector2D position)
{
	Entity* self;
	self = entity_new();
	if (!self) {
		slog("Failed to create floater entity");
		return NULL;
	}
	self->entity_type = "turret";
	self->sprite = gf2d_sprite_load_all(
		"images/turret.png",
		64,
		64,
		1,
		0
	);
	self->position = position;
	self->frame = 0;
	self->scale = gfc_vector2d(1, 1);
	self->think = turret_enemy_think;
	self->update = turret_enemy_update;
	self->velocity = gfc_vector2d(0, 0);
	self->collider = collider_new(gfc_rect(position.x, position.y, 64.0, 64.0), true, self);
	self->collide = turret_enemy_collide;
	Collider* collider = self->collider;
	collider->team = ENEMY;
	self->damager = 2;
	self->free = enemy_free;
	self->health = 30.0f;
	Enemy_Data* enemy_data = malloc(sizeof(Enemy_Data)); //used for damage etc
	if (enemy_data)
	{
		enemy_data->timer = 0;
		enemy_data->damage = 2.5f;
		self->data = enemy_data;
	}

	self->data = enemy_data;
	//self->collide = enemy_collide;
	return self;
}

//uses the same check_move, do not need a new one

void turret_enemy_think(Entity* self) {
	if (!self) {
		slog("enemies don't exist anymore");
		return;
	}
	
	Enemy_Data* enemy_data = self->data;
	Entity* my_player = player_get();
	GFC_Vector2D difference;
	gfc_vector2d_add(difference, my_player->position, -self->position);
	if ((!my_player) || (gfc_vector2d_magnitude(difference) > 500)) {

	}
	else if (enemy_data->timer <= 0)
	{
		enemy_data->timer = 90;
		Collider* collider = self->collider;
		GFC_Vector2D shotpoint = gfc_rect_get_center_point(collider->rect);
		GFC_Vector2D* shotdir = &difference;
		gfc_vector2d_normalize(shotdir);
		Entity* shot = shot_entity_new(shotpoint, *shotdir, 10.0f, PROJECTILE, 300);
		shot->sprite = gf2d_sprite_load_all(
			"images/shot_enemy.png",
			32,
			32,
			1,
			0
		);
		Collider* shot_collider = shot->collider;
		shot_collider->team = ENEMY;
	}

	else if (enemy_data->timer > 0)
	{
		enemy_data->timer -= 1;
	}
	/*	else {
			self->velocity.x = 0;
		}
	*/

}

void turret_enemy_update(Entity* self) {
	if (!self) {
		slog("enemies don't exist anymore");
		return;
	}
	GFC_Vector2D new_position = self->position;
	//new_position.x += self->velocity.x;
	//new_position.y += self->velocity.y;
	if (self->collider) {
		Collider* collider = self->collider;

		self->position = enemy_check_move(self, new_position);
	}

	if (self->collider) {
		Collider* collider = self->collider;
		if (collider->_inuse == 1) {
			collider->rect = gfc_rect(self->position.x, self->position.y, 64.0, 64.0);
		}
	}
}

void turret_enemy_free(Entity* self) {
	if (!self) return;
}

void turret_enemy_collide(Entity* self, void* collider) {
	{
		if (!collider) return;
		Collider* collided = collider;
		if (collided->team == 1) {
			if (collided->entity->damager == 1) {
				if (self->data == NULL) {
					return;
				}
				Shot_Data* damager = collided->entity->data;
				self->health -= damager->damage;

				if (self->health <= 0.0f) {
					audio_sfx_play("audio/sfx/enemyhurt2.wav");
					self->removeme = 1;
				}
				else
				{
					audio_sfx_play("audio/sfx/enemyhurt.wav");
					int dir = 1;
					if ((self->position.x - collided->entity->position.x) < 0) {
						dir = -1;
					}
					self->velocity = gfc_vector2d(dir * damager->damage * 0.5, -damager->damage * 0.5);
				}

			}
		}
	}
}

///////////////
//WALKY///////
///////////////
void walky_enemy_think(Entity* self);

void walky_enemy_update(Entity* self);

void walky_enemy_free(Entity* self);

void walky_enemy_collide(Entity* self, void* collider);


Entity* walky_enemy_entity_new(GFC_Vector2D position)
{
	Entity* self;
	self = entity_new();
	if (!self) {
		slog("Failed to create floater entity");
		return NULL;
	}
	self->entity_type = "walky";
	self->sprite = gf2d_sprite_load_all(
		"images/walky.png",
		64,
		64,
		1,
		0
	);
	self->position = position;
	self->frame = 0;
	self->scale = gfc_vector2d(1, 1);
	self->think = walky_enemy_think;
	self->update = walky_enemy_update;
	self->velocity = gfc_vector2d(0, 0);
	self->collider = collider_new(gfc_rect(position.x, position.y, 64.0, 64.0), true, self);
	self->collide = walky_enemy_collide;
	self->facing = 1;
	Collider* collider = self->collider;
	collider->team = ENEMY;
	self->damager = 2;
	self->free = enemy_free;
	self->health = 30.0f;
	Enemy_Data* enemy_data = malloc(sizeof(Enemy_Data)); //used for damage etc
	if (enemy_data)
	{
		enemy_data->timer = 120;
		enemy_data->damage = 5.0f;
		self->data = enemy_data;
	}
	//self->collide = enemy_collide;
	return self;
}

//uses the same check_move, do not need a new one

void walky_enemy_think(Entity* self) {
	if (!self) {
		slog("enemies don't exist anymore");
		return;
	}

	Enemy_Data* enemy_data = self->data;
	Entity* my_player = player_get();
	GFC_Vector2D difference;
	gfc_vector2d_add(difference, my_player->position, -self->position);
	
	
	Collider* collider = self->collider;
	if (enemy_data->timer <= 10)
	{
		
		GFC_Vector2D shotpoint = gfc_rect_get_center_point(collider->rect);
		shotpoint.y -= 16;
		shotpoint.x -= 16;
		GFC_Vector2D shotdir = gfc_vector2d(self->facing, 0);
		Entity* shot = shot_entity_new(shotpoint, shotdir, 5.0f, PROJECTILE, 300);
		shot->sprite = gf2d_sprite_load_all(
			"images/shot_enemy.png",
			32,
			32,
			1,
			0
		);
		Collider* shot_collider = shot->collider;
		shot_collider->team = ENEMY;
		
	}

	if (enemy_data->timer <= 0) {
		self->facing *= -1;
		enemy_data->timer = 120;
		self->scale.x = self->facing;
	}
	else if (enemy_data->timer > 0)
	{
		enemy_data->timer -= 1;
	}
	/*	else {
			self->velocity.x = 0;
		}
	*/
	self->velocity.x = 2 * self->facing;
	GFC_Rect ground_check_rect;
	ground_check_rect = gfc_rect(collider->rect.x, collider->rect.y + collider->rect.h, collider->rect.w, 2);
	if (collider_manager_check_static_collisions(ground_check_rect).h != 0) {
		self->velocity.y == 0.0f;
	}
	else
	{
		self->velocity.y += 0.1f;
	}
	

}

void walky_enemy_update(Entity* self) {
	if (!self) {
		slog("enemies don't exist anymore");
		return;
	}
	GFC_Vector2D new_position = self->position;
	new_position.x += self->velocity.x;
	new_position.y += self->velocity.y;
	if (self->collider) {
		Collider* collider = self->collider;

		self->position = enemy_check_move(self, new_position);
	}

	if (self->collider) {
		Collider* collider = self->collider;
		if (collider->_inuse == 1) {
			collider->rect = gfc_rect(self->position.x, self->position.y, 64.0, 64.0);
		}
	}
}

void walky_enemy_free(Entity* self) {
	if (!self) return;
}

void walky_enemy_collide(Entity* self, void* collider) {
	{
		if (!collider) return;
		Collider* collided = collider;
		if (collided->team == 1) {
			if (collided->entity->damager == 1) {
				if (self->data == NULL) {
					return;
				}
				Shot_Data* damager = collided->entity->data;
				self->health -= damager->damage;

				if (self->health <= 0.0f) {
					audio_sfx_play("audio/sfx/enemyhurt2.wav");
					self->removeme = 1;
				}
				else
				{
					audio_sfx_play("audio/sfx/enemyhurt.wav");
					int dir = 1;
					if ((self->position.x - collided->entity->position.x) < 0) {
						dir = -1;
					}
					self->velocity = gfc_vector2d(dir * damager->damage * 0.5, -damager->damage * 0.5);
				}

			}
		}
	}
}


///////////////
//BRUISER///////
///////////////
void bruiser_enemy_think(Entity* self);

void bruiser_enemy_update(Entity* self);

void bruiser_enemy_free(Entity* self);

void bruiser_enemy_collide(Entity* self, void* collider);


Entity* bruiser_enemy_entity_new(GFC_Vector2D position)
{
	Entity* self;
	self = entity_new();
	if (!self) {
		slog("Failed to create floater entity");
		return NULL;
	}
	self->sprite = gf2d_sprite_load_all(
		"images/bruiser.png",
		128,
		128,
		1,
		0
	);
	self->entity_type = "bruiser";
	self->position = position;
	self->frame = 0;
	self->scale = gfc_vector2d(1, 1);
	self->think = bruiser_enemy_think;
	self->update = bruiser_enemy_update;
	self->velocity = gfc_vector2d(0, 0);
	self->collider = collider_new(gfc_rect(position.x, position.y, 128.0, 128.0), true, self);
	self->collide = bruiser_enemy_collide;
	self->facing = 1;
	Collider* collider = self->collider;
	collider->team = ENEMY;
	self->damager = 2;
	self->free = enemy_free;
	self->health = 100.0f;
	Enemy_Data* enemy_data = malloc(sizeof(Enemy_Data)); //used for damage etc
	if (enemy_data)
	{
		enemy_data->timer = 120;
		enemy_data->damage = 10.0f;
		self->data = enemy_data;
	}
	//self->collide = enemy_collide;
	return self;
}

//uses the same check_move, do not need a new one

void bruiser_enemy_think(Entity* self) {
	if (!self) {
		slog("enemies don't exist anymore");
		return;
	}

	Enemy_Data* enemy_data = self->data;
	Entity* my_player = player_get();
	GFC_Vector2D difference;
	gfc_vector2d_add(difference, my_player->position, -self->position);


	Collider* collider = self->collider;
	if (enemy_data->timer <= 10)
	{

		GFC_Vector2D shotpoint = gfc_rect_get_center_point(collider->rect);
		shotpoint.y -= 64.0f;
		shotpoint.x += -64 + (128.0f * (float)self->facing);
		GFC_Vector2D shotdir = gfc_vector2d(self->facing, 0);
		Entity* shot = shot_entity_new(shotpoint, shotdir, 15.0f, MELEE, 3);
		shot->sprite = gf2d_sprite_load_all(
			"images/shot_enemy.png",
			32,
			32,
			1,
			0
		);
		shot->scale = gfc_vector2d(4, 4);
		Collider* shot_collider = shot->collider;
		shot_collider->rect.h *= 4;
		shot_collider->rect.w *= 4;
		shot_collider->team = ENEMY;

	}

	if (enemy_data->timer <= 0) {
		self->facing *= -1;
		enemy_data->timer = 180;
		self->scale.x = self->facing;
	}
	else if (enemy_data->timer > 0)
	{
		enemy_data->timer -= 1;
	}
	/*	else {
			self->velocity.x = 0;
		}
	*/
	self->velocity.x = self->facing;
	GFC_Rect ground_check_rect;
	ground_check_rect = gfc_rect(collider->rect.x, collider->rect.y + collider->rect.h, collider->rect.w, 2);
	if (collider_manager_check_static_collisions(ground_check_rect).h != 0) {
		self->velocity.y == 0.0f;
	}
	else
	{
		self->velocity.y += 0.1f;
	}


}

void bruiser_enemy_update(Entity* self) {
	if (!self) {
		slog("enemies don't exist anymore");
		return;
	}
	GFC_Vector2D new_position = self->position;
	new_position.x += self->velocity.x;
	new_position.y += self->velocity.y;
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

void bruiser_enemy_free(Entity* self) {
	if (!self) return;
}

void bruiser_enemy_collide(Entity* self, void* collider) {
	{
		if (!collider) return;
		Collider* collided = collider;
		if (collided->team == 1) {
			if (collided->entity->damager == 1) {
				if (self->data == NULL) {
					return;
				}
				Shot_Data* damager = collided->entity->data;
				self->health -= damager->damage;

				if (self->health <= 0.0f) {
					audio_sfx_play("audio/sfx/enemyhurt2.wav");
					self->removeme = 1;
				}
				else
				{
					audio_sfx_play("audio/sfx/enemyhurt.wav");
					int dir = 1;
					if ((self->position.x - collided->entity->position.x) < 0) {
						dir = -1;
					}
					self->velocity = gfc_vector2d(dir * damager->damage * 0.5, -damager->damage * 0.5);
				}

			}
		}
	}
}


/*eol@eof*/
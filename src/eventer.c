#include "simple_logger.h"
#include "gfc_input.h"

#include "collider.h"
#include "eventer.h"
#include "gui.h"
#include "quest.h"

void eventer_think(Entity* self);
void eventer_update(Entity* self);
void eventer_collide(Entity* self, void* collider);
void eventer_free(Entity* self);



Entity* eventer_entity_new(GFC_Vector2D position, GFC_Vector2D size, Sprite* sprite, void* ontouch) {
	Entity* self;
	self = entity_new();
	if (!self) {
		slog("failed to create event entity");
		return NULL;
	}
	self->entity_type = "event";
	self->sprite = sprite;
	self->position = position;
	self->collide = eventer_collide;
	self->scale = gfc_vector2d(1, 1);
	self->frame = 0;
	self->collider = collider_new(gfc_rect(position.x, position.y, size.x, size.y),true,self);
	self->think = eventer_think;
	self->update = eventer_update;
	Collider* collider = self->collider;
	collider->team = ENEMY;
	self->damager = 0;
	self->free = eventer_free;
	self->health = 1.0f;

	EventerData* mydata = malloc(sizeof(EventerData));
	mydata->num = 0;
	mydata->ontouch = ontouch;
	self->data = mydata;
	return self;
}

Entity* quester_entity_new(GFC_Vector2D position, int quest) {
	Entity* myQuester = NULL;
	if (quest == QUEST_JUMP)
	{
		Sprite* eventersprite = gf2d_sprite_load_all("images/eventer1.png",64,64,1,1);
		myQuester = eventer_entity_new(position, gfc_vector2d(64, 64), eventersprite, quest_interact_jump);
		myQuester->entity_type = "quest_jump";
	}
	else if (quest == QUEST_FLOATER)
	{
		Sprite* eventersprite = gf2d_sprite_load_all("images/eventer2.png", 64, 64, 1, 1);
		myQuester = eventer_entity_new(position, gfc_vector2d(64, 64), eventersprite, quest_interact_floater);
		myQuester->entity_type = "quest_floater";
	}
	else if (quest == QUEST_TALK)
	{
		Sprite* eventersprite = gf2d_sprite_load_all("images/eventer3.png", 64, 64, 1, 1);
		myQuester = eventer_entity_new(position, gfc_vector2d(64, 64), eventersprite, quest_interact_talk1);
		myQuester->entity_type = "quest_talk";
	}
	else if (quest == -1)
	{
		Sprite* eventersprite = gf2d_sprite_load_all("images/eventer4.png", 64, 64, 1, 1);
		myQuester = eventer_entity_new(position, gfc_vector2d(64, 64), eventersprite, quest_interact_talk2);
		myQuester->entity_type = "quest_talk_to";
	}
	slog("i'm quester and i exist");
	return myQuester;
	
	
}


void eventer_think(Entity* self)
{

}

void eventer_update(Entity* self) {

}


void eventer_collide(Entity* self, void* collider)
{
	if (!self)return;
	if (!collider)return;
	Collider* collided = collider;
	if (collided->isDynamic)
	{
		if (collided->entity->damager == 0 && collided->team == PLAYER) {
			EventerData* mydata = self->data;
			mydata->ontouch(self);
		}
	}
}

void eventer_free(Entity* self)
{
	if (!self)return;
}
#include "simple_logger.h"
#include "gfc_input.h"

#include "collider.h"
#include "speaker.h"
#include "gui.h"

void speaker_think(Entity* self);
void speaker_update(Entity* self);
void speaker_collide(Entity* self, void* collider);
void speaker_free(Entity* self);

Entity* speaker_entity_new(GFC_Vector2D position)
{
	Entity* self;
	self = entity_new();
	if (!self) {
		slog("failed to create speaker");
		return NULL;
	}
	self->sprite = gf2d_sprite_load_all(
		"images/speaker.png",
		64,
		64,
		1,
		0
	);
	self->position = position;
	self->frame = 0;
	self->scale = gfc_vector2d(1,1);
	self->collider = collider_new(gfc_rect(position.x, position.y, 64, 64), true, self);
	self->collide = speaker_collide;
	self->update = speaker_update;
	self->think = speaker_think;
	Collider* collider = self->collider;
	collider->team = ENEMY;
	self->damager = 0;
	self->free = speaker_free;
	self->health = 1.0f;
	Uint8* timer = malloc(sizeof(Uint8));
	timer = 0;
	self->data = timer;
	self->solid = 0;
	self->removeme = 0;
	slog("finished becoming speaker");
}

void speaker_think(Entity* self) {
	Uint8* timer = self->data;
	slog("my timer is %d", timer);
	if (timer == 300)
	{
		write_dialogue("that's right it's ME your BEST BUD CLAMLET");
	}
	if (timer == 600) {
		write_dialogue("do you think you're a PEARL of the world???");
	}
	if (timer == 900)
	{
		write_choices("not particularly", "i'm the coolest...");
	}
	if (timer == 1500) {
		hide_dialogue();
		timer = 0;
	}
	if (timer == 901)
	{
		if (gfc_input_key_pressed("a"))
		{
			write_dialogue("chin up you FREAKY FRIEND! you'll make it!!");
			timer += 1;
		}
		else if (gfc_input_key_pressed("d"))
		{
			write_dialogue("THAT'S the spirit! tally ho, you jellylass!");
			timer += 1;
		}
	}
	else if (timer > 0)
	{
		timer += 1;
	}

	
	self->data = timer;
}

void speaker_update(Entity* self) {
}

void speaker_collide(Entity* self, void* collider)
{
	if (!self)return;
	if (!collider)return;
	Collider* collided = collider;
	if (collided->isDynamic)
	{
		Uint8* timer = self->data;
		if (collided->entity->damager == 0 && collided->team == PLAYER && timer == 0) {
			write_dialogue("HELLO!!! i am talking to YOU!!!!!!! my FRIEND");
			Uint16* timer = 1;
			self->data = timer;
		}
	}
}

void speaker_free(Entity* self)
{
	if (!self)return;
}
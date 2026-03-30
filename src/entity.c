#include "simple_logger.h"

#include "gfc_types.h"
#include "gf2d_sprite.h"

#include "entity.h"
#include "camera.h"

typedef struct
{
	Entity *entityList;
	Uint32 entityMax;
}EntityManager;

static EntityManager entityManager = {0}; /**<intiialize a LOCAL global entity manager*/

void postupdate(Entity* self);

void entity_manager_init(Uint32 max)
{
	if (!max)
	{
		slog("cannot initialize entity system with zero entities");
		return;
	}
	entityManager.entityList = malloc(sizeof(Entity)*max);
	if (!entityManager.entityList)
	{
		slog("failed to allocate %i entities", max);
		return;
	}
	entityManager.entityMax = max;
	atexit(entity_manager_close);
	slog("initialized entity system");
}

void entity_manager_close()
{
	int i;
	if (!entityManager.entityList)return;
	for (i = 0; i < entityManager.entityMax;i++)
	{
		entity_free( &entityManager.entityList[i]);
	}
	free(entityManager.entityList);
	memset(&entityManager,0,sizeof(EntityManager));
	slog("closed entity system");
}

Entity *entity_new()
{
	
	int i;
	if (!entityManager.entityList)
	{
		slog("entity system has not been initialized!");
		return NULL;
	}
	for (i = 0; i < entityManager.entityMax;i++){
		if (entityManager.entityList[i]._inuse == 1)continue;
		entityManager.entityList[i]._inuse = 1;
		//set defaults
		entityManager.entityList[i].scale.x = 1;
		entityManager.entityList[i].scale.y = 1;
		entityManager.entityList[i].postupdate = postupdate;
		return &entityManager.entityList[i];
	}
	return NULL;
	
	
}

void entity_draw(Entity *self) {
	if (!self)return;
	if (self->sprite) {
		GFC_Vector2D new_position;
		gfc_vector2d_add(new_position, self->position, camera_get_offset());
		gf2d_sprite_draw(
			self->sprite,
			new_position,
			&self->scale,
			NULL,
			NULL,
			NULL,
			NULL,
			(Uint32)self->frame);
	}
}


void entity_free(Entity *self)
{
	if (!self)return;
	if (self->sprite)gf2d_sprite_free(self->sprite);
	if (self->free)self->free(self);
	if (self->collider)
	{
		collider_free(self->collider);
	}
	//this is an issue, figure out how to clean up the collider later

	
	memset(self,0,sizeof(Entity));
}

void entity_manager_think_all() {
	int i;
	if (!entityManager.entityList)return;
	for (i = 0; i < entityManager.entityMax; i++)
	{
		if (entityManager.entityList[i]._inuse != 1)continue;
		if (entityManager.entityList[i].think)entityManager.entityList[i].think(&entityManager.entityList[i]);
	}

}

void entity_manager_update_all() {
	int i;
	if (!entityManager.entityList)return;
	for (i = 0; i < entityManager.entityMax; i++)
	{
		if (entityManager.entityList[i]._inuse != 1)continue;
		if (entityManager.entityList[i].update != NULL)entityManager.entityList[i].update(&entityManager.entityList[i]);
		if (entityManager.entityList[i].postupdate != NULL)entityManager.entityList[i].postupdate(&entityManager.entityList[i]);
	}

}


void entity_manager_draw_all() {
	int i;
	if (!entityManager.entityList)
	{
		slog("entity system has not been initialized!");
		return;
	}
	for (i = 0; i < entityManager.entityMax;i++)
	{
		if (entityManager.entityList[i]._inuse == 1) {
			entity_draw(&entityManager.entityList[i]);
		}
		
	}
}

void postupdate(Entity* self) {
	if (self->removeme == 1) {
		entity_free(self);
	}
}

/*eol@eof*/
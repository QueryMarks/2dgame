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
}

void entity_manager_close()
{
	int i;
	if (!entityManager.entityList)return;
	for (i = 0; i < entityManager.entityMax;i++)
	{
		if (&entityManager.entityList[i]._inuse == 1)
		{
			entity_free(&entityManager.entityList[i]);
		}
		
	}
	free(entityManager.entityList);
	memset(&entityManager,0,sizeof(EntityManager));
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
		entityManager.entityList[i].entity_type = "EMPTY_ENTITY";
		return &entityManager.entityList[i];
	}
	return NULL;
	
	
}

void entity_draw(Entity *self) {
	if (!self)return;
	if (self->sprite) {
		GFC_Vector2D new_position;
		gfc_vector2d_add(new_position, self->position, camera_get_offset());
		GFC_Vector2D flip = gfc_vector2d(0,0);
		if (self->facing == -1)flip = gfc_vector2d(1,0);
		GFC_Vector2D* flipptr = &flip;

		gf2d_sprite_draw(
			self->sprite,
			new_position,
			&self->scale,
			NULL,
			NULL,
			flipptr,
			NULL,
			(Uint32)self->frame);
	}
}

SJson* entities_for_json() {
	if (!entityManager.entityList)
	{
		slog("entity system has not been initialized!");
		return NULL;
	}
	SJson* entities = sj_array_new();
	for (int i = 0; i < entityManager.entityMax; i++) {
		if (entityManager.entityList[i]._inuse != 1)continue;
		if (strcmp(entityManager.entityList[i].entity_type, "player") == 0
			|| strcmp(entityManager.entityList[i].entity_type, "shot") == 0
			|| strcmp(entityManager.entityList[i].entity_type, "EMPTY_ENTITY")==0)continue;

		SJson* entity_json = sj_object_new();
		SJson* entity = sj_new_str(entityManager.entityList[i].entity_type);
		SJson* pos_x = sj_new_float(entityManager.entityList[i].position.x);
		SJson* pos_y = sj_new_float(entityManager.entityList[i].position.y);

		sj_object_insert(entity_json, "entity", entity);
		sj_object_insert(entity_json, "pos_x", pos_x);
		sj_object_insert(entity_json, "pos_y", pos_y);
		sj_array_append(entities, entity_json);
	}
	return entities;
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

void clear_entities_at_point(GFC_Vector2D point)
{
	for (int i = 0; i < entityManager.entityMax; i++)
	{
		if (entityManager.entityList[i]._inuse == 1)
		{
			if (strcmp(entityManager.entityList[i].entity_type, "player") != 0) {
				GFC_Rect rect = gfc_rect(entityManager.entityList[i].position.x, entityManager.entityList[i].position.y, 64, 64);
				if (gfc_point_in_rect(point, rect)) {
					entityManager.entityList[i].free(&entityManager.entityList[i]);
					entityManager.entityList[i]._inuse = 0;
				}
			}
		}
		
	}
}

/*eol@eof*/
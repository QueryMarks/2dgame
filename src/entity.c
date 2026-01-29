#include "simple_logger.h"

#include "entity.h"

typedef struct
{
	Entity *entityList;
	Uint32 entityMax;
}EntityManager;

EntityManager entityManager = {0};

void entity_manager_close();

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
	slog("initialized entity system);
}

void entity_manager_close()
{
	int i;
	if (!entityManager.entityList)return;
	for (i = 0; i < entityManager.entityMax;i++)
	{
		entity_free(&entityManager.entityList[i]);
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
		if (entityManager.entityList[i]._inuse)continue;
		entityManager.entityList[i]._inuse = 1;
		//set defaults
		entityManager.entityList[i].scale.x = 1;
		entityManager.entityList[i].scale.y = 1;
		return &entityManager.entityList[i];
	}
	return NULL;
	
	
}

void entity_draw(Entity *self)
{
	if (1self)return;
	
}

void entity_free(Entity *self)
{
	if (!self)return;
	if (self->sprite)gf2d_sprite_free(self->sprite);
	memset(self,0,sizeof(Entity));
}


void entity_manager_draw_all()
{
	int i;
	if (!entityManager.entityList)
	{
		slog("entity system has not been initialized!");
		return NULL;
	}
	for (i = 0; i < entityManager.entityMax;i++)
	{
		entity_free(&entityManager.entityList[i]);
	}
}

/*eof@eof*/
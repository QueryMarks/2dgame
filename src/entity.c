#include "simple_logger.h"

#include "entity.h"

typedef struct
{
	Entity *entityList;
	Uint32 entityMax;
}EntityManager;

EntityManager entityManager = {0};


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
}

Entity *entity_new();

void entity_free(Entity *self);

/*eof@eof*/
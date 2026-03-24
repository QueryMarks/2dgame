#include "simple_logger.h"

#include "gfc_types.h"
#include "gf2d_sprite.h"

#include "entity.h"
#include "camera.h"
#include "collider.h"


typedef struct {
	Collider* colliderList;
	Uint32 colliderMax;
}ColliderManager;

static ColliderManager colliderManagerStatic = { 0 };
static ColliderManager colliderManagerDynamic = { 0 };

void collider_manager_init(Uint32 max, Bool isDynamic) {
	if (!max) {
		slog("cannot initialize collision manager without a max");
		return;
	}
	ColliderManager colliderManager;
	if (isDynamic) {
		colliderManager = colliderManagerDynamic;
	}
	else
	{
		colliderManager = colliderManagerStatic;
	}
	colliderManagerDynamic.colliderList = malloc(sizeof(Collider) * max);
	colliderManagerStatic.colliderList = malloc(sizeof(Collider) * max);
	if (!colliderManagerDynamic.colliderList || !colliderManagerStatic.colliderList) {
		slog("no collider lists you buffoon");
		return;
	}
	colliderManagerDynamic.colliderMax = max;
	colliderManagerStatic.colliderMax = max;
	atexit(collider_manager_close);
	slog("initialized collider manager. it is dynamic? %d", isDynamic);	
}
void collider_free(Collider* self) {
	if (!self)return;
	//if (self->collider_free)self->collider_free(self);
	memset(self, 0, sizeof(Collider));
}
void collider_manager_close() {
	int i;
	if (!colliderManagerDynamic.colliderList || !colliderManagerStatic.colliderList)return;
	for (i = 0; i < colliderManagerDynamic.colliderMax; i++)
	{
		collider_free(&colliderManagerDynamic.colliderList[i]);
	}
	free(colliderManagerDynamic.colliderList);
	for (i = 0; i < colliderManagerStatic.colliderMax; i++)
	{
		collider_free(&colliderManagerStatic.colliderList[i]);
	}
	free(colliderManagerStatic.colliderList);

	memset(&colliderManagerDynamic, 0, sizeof(ColliderManager));
	memset(&colliderManagerStatic, 0, sizeof(ColliderManager));
}

Uint8 checkCollision(struct Collider_S self, struct Collider_S other) {
	
	Bool result = gfc_rect_overlap(self.rect, other.rect);
	return result;
}
void collider_manager_check_collisions() {
	int i, j;
	for (j = 0; j < colliderManagerDynamic.colliderMax; j++) {
		if (colliderManagerDynamic.colliderList[j]._inuse != 1)return;
		for (i = j + 1; i < colliderManagerDynamic.colliderMax; i++) {
			if (colliderManagerDynamic.colliderList[i]._inuse != 1)return;
			//slog("one collider's position is %f, %f", colliderManagerDynamic.colliderList[i].rect.x, colliderManagerDynamic.colliderList[i].rect.y);
			//slog("one collider's position is %f, %f", colliderManagerDynamic.colliderList[j].rect.x, colliderManagerDynamic.colliderList[j].rect.y);

			if (checkCollision(colliderManagerDynamic.colliderList[j], colliderManagerDynamic.colliderList[i])) {
				slog("have the objects touched? YES!!!");
				if ((colliderManagerDynamic.colliderList[j].entity != NULL) && (colliderManagerDynamic.colliderList[i].entity != NULL))
					entity_free(colliderManagerDynamic.colliderList[i].entity);
			}
			else {
				
			}
			
		}
	}
}

//returns a new collider
Collider* collider_new(GFC_Rect rect, Bool isDynamic, Entity* entity) {
	//if the collider is dynamic do the appropriate stuff for that
	int i;
	ColliderManager *colliderManager;
	slog("we made i to collider_new without crashing");
	if (isDynamic) {
		colliderManager = &colliderManagerDynamic;
	}
	//if not
	else {
		colliderManager = &colliderManagerStatic;
	}
	if (!colliderManager->colliderList) {
		slog("no collider manager list yet!");
		return NULL;
	}
	for (i = 0; i < colliderManager->colliderMax; i++) {
		if (colliderManager->colliderList[i]._inuse == 1)continue;
		colliderManager->colliderList[i]._inuse = 1;
		colliderManager->colliderList[i].isDynamic = isDynamic;
		colliderManager->colliderList[i].rect = rect;
		colliderManager->colliderList[i].entity = entity;
		colliderManager->colliderList[i].free = collider_free;
		return &colliderManager->colliderList[i];
	}
	return NULL;
}

void collider_draw_all() {
	int i, j;
	for (j = 0; j < colliderManagerDynamic.colliderMax; j++) {
		if (colliderManagerDynamic.colliderList[j]._inuse != 1)return;
	}
}




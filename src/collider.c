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
	memset(self, 0, sizeof(Collider));
	//if (self->collider_free)self->collider_free(self);
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
		if (colliderManagerDynamic.colliderList[j]._inuse != 1)continue;
		for (i = j + 1; i < colliderManagerDynamic.colliderMax; i++) {
			if (colliderManagerDynamic.colliderList[i]._inuse != 1)continue;
			//slog("one collider's position is %f, %f", colliderManagerDynamic.colliderList[i].rect.x, colliderManagerDynamic.colliderList[i].rect.y);
			//slog("one collider's position is %f, %f", colliderManagerDynamic.colliderList[j].rect.x, colliderManagerDynamic.colliderList[j].rect.y);

			if (checkCollision(colliderManagerDynamic.colliderList[j], colliderManagerDynamic.colliderList[i])) {
				if ((colliderManagerDynamic.colliderList[j].entity != NULL) && (colliderManagerDynamic.colliderList[i].entity != NULL))
				{
					colliderManagerDynamic.colliderList[j].entity->collide(colliderManagerDynamic.colliderList[j].entity, &colliderManagerDynamic.colliderList[i]);
				}
				if ((colliderManagerDynamic.colliderList[j].entity != NULL) && (colliderManagerDynamic.colliderList[i].entity != NULL))
				{
					colliderManagerDynamic.colliderList[i].entity->collide(colliderManagerDynamic.colliderList[i].entity, &colliderManagerDynamic.colliderList[j]);
				}
			}
			else {
				
			}
			
		}
		//dynamic collisions with static colliders
		for (i = 0; i < colliderManagerStatic.colliderMax; i++) {
			if (colliderManagerStatic.colliderList[i]._inuse != 1)continue;
			if (checkCollision(colliderManagerDynamic.colliderList[j], colliderManagerStatic.colliderList[i])) {
				if (!colliderManagerDynamic.colliderList[j].entity)continue;
				if (colliderManagerDynamic.colliderList[j].entity->collide) {
					colliderManagerDynamic.colliderList[j].entity->collide(colliderManagerDynamic.colliderList[j].entity, &colliderManagerStatic.colliderList[i]);
				}
			}
		}
	}
}

//commandeered rect overlap exclusive
Uint8 gfc_rect_overlap_excl(GFC_Rect a, GFC_Rect b)
{
	int ax = round(a.x);
	int aw = round(a.w);
	int ay = round(a.y);
	int ah = round(a.h);
	int bx = round(b.x);
	int bw = round(b.w);
	int by = round(b.y);
	int bh = round(b.h);
	if (((a.x >= b.x + b.w)) || //(ax = bx+bw)) ||
		((b.x >= a.x + a.w)) || //(bx = round(ax+aw))) ||
		((a.y >= b.y + b.h)) || //(ay = round(by+bh))) ||
		((b.y >= a.y + a.h))) //|| (by = round(ay+ah))))
	{
		
		return 0;
	}
	else
	{
		//slog("rect a is %d %d %d %d", ax, aw, ay, ah);
		//slog("rect b is %d %d %d %d", bx, bw, by, bh);
		return 1;
	}
}
//Returns a pointer to the first overlapping collider in the list if the given rect would intersect with a static collider. Returns NULL if no collider would 
GFC_Rect collider_manager_check_static_collisions(GFC_Rect rect) {
	//Collider *collider = collider_new(rect, true, NULL);
	for (int i = 0; i < colliderManagerStatic.colliderMax; i++) {
		if (colliderManagerStatic.colliderList[i]._inuse != 1)continue;
		if (gfc_rect_overlap_excl(rect, colliderManagerStatic.colliderList[i].rect)) {
			return colliderManagerStatic.colliderList[i].rect;
		}
	}
	return (gfc_rect(0,0,0,0));

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
		colliderManager->colliderList[i].team = 0;
		return &colliderManager->colliderList[i];
	}
	return NULL;
}

void collider_manager_draw_all() {
	int i, j;
	for (j = 0; j < colliderManagerDynamic.colliderMax; j++) {
		if (colliderManagerDynamic.colliderList[j]._inuse != 1)continue;
		GFC_Rect camera_rect;
		gfc_rect_copy(camera_rect, colliderManagerDynamic.colliderList[j].rect);
		camera_rect.x = camera_rect.x + camera_get_offset().x;
		camera_rect.y = camera_rect.y + camera_get_offset().y;
		gf2d_draw_rect(camera_rect, gfc_color(1, 0, 0, 1));
	}
	for (j = 0; j < colliderManagerStatic.colliderMax; j++) {
		if (colliderManagerStatic.colliderList[j]._inuse != 1)continue;
		GFC_Rect camera_rect;
		gfc_rect_copy(camera_rect, colliderManagerStatic.colliderList[j].rect);
		camera_rect.x = camera_rect.x + camera_get_offset().x;
		camera_rect.y = camera_rect.y + camera_get_offset().y;
		gf2d_draw_rect(camera_rect, gfc_color(1, 0, 0, 1));
	}
}




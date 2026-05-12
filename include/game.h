#include "enemy.h"
#include "level.h"
#include "camera.h"
#include "collider.h"
#include "shot.h"
#include "wall.h"
#include "current.h"
#include "gui.h"
#include "spike.h"
#include "tnt.h"
#include "damagezone.h"
#include "speaker.h"


#include "window.h"
#include "element.h"


enum Gamestate_E{
	GS_TITLE,
	GS_MAIN,
	GS_EDITOR
};

typedef struct Game_Manager_S {
	Level* level;
	Entity* player;
	int time;
	int gamestate;
}Game_Manager;

void game_start_title();
void game_title_exit();

void game_start_level(const char* path);
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
	TITLE,
	MAIN,
	EDITOR
};

typedef struct Game_Manager_S {
	Level* level;
	Entity* player;
	int time;
	int gamestate;
}Game_Manager;

void game_load_title() {

}

void game_load_level(const char* path) {

}
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
	GS_EDITOR,
	GS_L2L
};

typedef struct Game_Manager_S {
	Level* level;
	Entity* player;
	int time;
	int gamestate;
	const char* level_to_level;
}Game_Manager;


typedef struct Game_State_Info_S
{
	float player_health;
	int player_weapon;
}Game_State_Info;

void game_start_title();
void game_title_exit();

void game_start_level(const char* path);
void game_level_to_level(const char* level);

int get_gamestate();
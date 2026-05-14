#include "editor.h"
#include "simple_logger.h"
#include "gf2d_sprite.h"

#include "enemy.h"
#include "spike.h"
#include "damagezone.h"
#include "speaker.h"
#include "tnt.h"
#include "wall.h"
#include "current.h"
#include "eventer.h"
#include "quest.h"

#include "level.h"
#include "camera.h"

static Editor _editor = {0};

enum {
	ED_WALKY,
	ED_BRUISER,
	ED_TURRET,
	ED_FLOATER,
	ED_SWINGER,
	ED_SPIKE,
	ED_DAMAGEZONE,
	ED_SPEAKER,
	ED_TNT,
	ED_WALL_EXPLOSIVE,
	ED_WALL_MELEE,
	ED_CURRENT,
	ED_QUEST_JUMP,
	ED_QUEST_FLOATER,
	ED_QUEST_TALK,
	ED_QUEST_TALK_TO,
	ED_LEVELER
};

void editor_init(Level* level)
{
	_editor.entity_index = 0;
	_editor.tile_index = 0;
	Sprite* walky = gf2d_sprite_load_all("images/walky.png",64,64,1,1);
	_editor.sprite_array = gfc_list_new();
	gfc_list_append(_editor.sprite_array, walky);
	gfc_list_append(_editor.sprite_array, gf2d_sprite_load_all("images/bruiser.png", 128, 128, 1, 1));
	gfc_list_append(_editor.sprite_array, gf2d_sprite_load_image("images/turret.png"));
	gfc_list_append(_editor.sprite_array, gf2d_sprite_load_image("images/floater.png"));
	gfc_list_append(_editor.sprite_array, gf2d_sprite_load_image("images/swinger.png"));
	gfc_list_append(_editor.sprite_array, gf2d_sprite_load_image("images/spike.png"));
	gfc_list_append(_editor.sprite_array, gf2d_sprite_load_image("images/damagezone.png"));
	gfc_list_append(_editor.sprite_array, gf2d_sprite_load_image("images/speaker.png"));

	gfc_list_append(_editor.sprite_array, gf2d_sprite_load_all("images/tnt.png",64,64,1,1));
	gfc_list_append(_editor.sprite_array, gf2d_sprite_load_image("images/wall_explosive.png"));

	gfc_list_append(_editor.sprite_array, gf2d_sprite_load_image("images/wall_melee.png"));
	gfc_list_append(_editor.sprite_array, gf2d_sprite_load_image("images/current.png"));



	gfc_list_append(_editor.sprite_array, gf2d_sprite_load_image("images/eventer1.png"));

	gfc_list_append(_editor.sprite_array, gf2d_sprite_load_image("images/eventer2.png"));

	gfc_list_append(_editor.sprite_array, gf2d_sprite_load_image("images/eventer3.png"));

	gfc_list_append(_editor.sprite_array, gf2d_sprite_load_image("images/eventer4.png"));
	gfc_list_append(_editor.sprite_array, gf2d_sprite_load_image("images/leveler.png"));


	Sprite* tiles = level->tileSet;

	Window* level_editor_window = window_new();
	level_editor_window->sprite = gf2d_sprite_load_all("images/editorbox.png", 450, 898, 1, 1);
	level_editor_window->position = gfc_vector2d(798, 0);

	_editor.entity_button =
		button_new(level_editor_window, gfc_vector2d(100, 300), walky, gfc_vector2d(64, 64), editor_change_entity);
	_editor.tile_button =
		button_new(level_editor_window, gfc_vector2d(100, 500), tiles, gfc_vector2d(64, 64), editor_change_tile);

	_editor.entities_max = _editor.sprite_array->count-1;
	_editor.tiles_max = 4;
	
	_editor.level = level;

}

void editor_change_entity()
{
	if (_editor.entity_index < _editor.entities_max)
	{
		_editor.entity_index += 1;
	}
	else
	{
		_editor.entity_index = 0;
	}
	_editor.entity_button->sprite = gfc_list_get_nth(_editor.sprite_array, _editor.entity_index);
	
}

void editor_change_tile()
{
	if (_editor.tile_index < _editor.tiles_max)
	{
		_editor.tile_index += 1;
		
	}
	else
	{
		_editor.tile_index = 0;
	}
	_editor.tile_button->frame = _editor.tile_index;
	
}



void editor_place_entity(GFC_Vector2D position) {
	if (_editor.entity_index == ED_WALKY)
	{
		walky_enemy_entity_new(position);
	}
	else if (_editor.entity_index == ED_BRUISER)
	{
		bruiser_enemy_entity_new(position);
	}
	else if (_editor.entity_index == ED_DAMAGEZONE)
	{
		damagezone_entity_new(position, 40, gfc_vector2d(64, 64));
	}
	else if (_editor.entity_index == ED_SPEAKER)
	{
		speaker_entity_new(position);
	}
	else if (_editor.entity_index == ED_SPIKE)
	{
		spike_entity_new(position,10,gfc_vector2d(64,64));
	}
	else if (_editor.entity_index == ED_FLOATER)
	{
		floater_enemy_entity_new(position);
	}
	else if (_editor.entity_index == ED_SWINGER)
	{
		enemy_entity_new(position);
	}
	else if (_editor.entity_index == ED_TNT)
	{
		tnt_entity_new(position);
	}
	else if (_editor.entity_index == ED_TURRET)
	{
		turret_enemy_entity_new(position);
	}
	else if (_editor.entity_index == ED_WALL_EXPLOSIVE)
	{
		wall_entity_new(position,2,gfc_vector2d(64,128));
	}
	else if (_editor.entity_index == ED_WALL_MELEE)
	{
		wall_entity_new(position, 1, gfc_vector2d(64, 128));
	}
	else if (_editor.entity_index == ED_CURRENT)
	{
		current_entity_new(position,gfc_vector2d(0,1),gfc_vector2d(64,128));
	}
	else if (_editor.entity_index == ED_QUEST_JUMP)
	{
		quester_entity_new(position, QUEST_JUMP);
	}
	else if (_editor.entity_index == ED_QUEST_FLOATER)
	{
		quester_entity_new(position, QUEST_FLOATER);
	}
	else if (_editor.entity_index == ED_QUEST_TALK)
	{
		quester_entity_new(position, QUEST_TALK);
	}
	else if (_editor.entity_index == ED_QUEST_TALK_TO)
	{
		quester_entity_new(position, -1);
	}
	else if (_editor.entity_index == ED_LEVELER)
	{
		level_eventer_entity_new(position);
	}
}


void editor_place_tile(int tile_x, int tile_y) {
	int index = tile_x + (tile_y * _editor.level->tileMapWidth);
	_editor.level->tileMap[index] = _editor.tile_index + 1;
	level_refresh_tiles(_editor.level);
}

void editor_clear(int mx_offset, int my_offset)
{

	int tile_x = mx_offset / 64;
	int tile_y = my_offset / 64;
	if (tile_x >= 0
		&& tile_x < _editor.level->tileMapWidth
		&& tile_y >= 0
		&& tile_y < _editor.level->tileMapHeight)
	{
		int index = tile_x + (tile_y * _editor.level->tileMapWidth);
		_editor.level->tileMap[index] = 0;
		level_refresh_tiles(_editor.level);
	}
	clear_entities_at_point(gfc_vector2d(mx_offset, my_offset));

}
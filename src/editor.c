#include "editor.h"
#include "simple_logger.h"
#include "gf2d_sprite.h"

#include "enemy.h"
#include "level.h"
#include "camera.h"

static Editor _editor = {0};

void editor_init(Level* level)
{
	slog("started initializing editor");
	_editor.entity_index = 0;
	_editor.tile_index = 0;
	Sprite* walky = gf2d_sprite_load_all("images/walky.png",64,64,1,1);
	slog("before new list");
	_editor.sprite_array = gfc_list_new();
	slog("initialized sprite array");
	gfc_list_append(_editor.sprite_array, walky);
	gfc_list_append(_editor.sprite_array, gf2d_sprite_load_all("images/bruiser.png", 128, 128, 1, 1));
	slog("checkpoint 1 editor init");
	Sprite* tiles = level->tileSet;

	Window* level_editor_window = window_new();
	level_editor_window->sprite = gf2d_sprite_load_all("images/editorbox.png", 450, 898, 1, 1);
	level_editor_window->position = gfc_vector2d(798, 0);
	slog("checkpoint 1.5");
	_editor.entity_button =
		button_new(level_editor_window, gfc_vector2d(100, 300), walky, gfc_vector2d(64, 64), editor_change_entity);
	_editor.tile_button =
		button_new(level_editor_window, gfc_vector2d(100, 500), tiles, gfc_vector2d(64, 64), editor_change_tile);
	slog("checkpoint 2 editor init");
	_editor.entities_max = _editor.sprite_array->count-1;
	_editor.tiles_max = 4;
	
	_editor.level = level;
	slog("initialized editor");
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

enum {
	ED_WALKY,
	ED_BRUISER
};

void editor_place_entity(GFC_Vector2D position) {
	if (_editor.entity_index == ED_WALKY)
	{
		walky_enemy_entity_new(position);
	}
	else if (_editor.entity_index == ED_BRUISER)
	{
		bruiser_enemy_entity_new(position);
	}
}


void editor_place_tile(int tile_x, int tile_y) {
	slog("building tile at %i, %i", tile_x, tile_y);
	int index = tile_x + (tile_y * _editor.level->tileMapWidth);
	_editor.level->tileMap[index] = _editor.tile_index + 1;
	level_refresh_tiles(_editor.level);
}

void editor_clear(int mx_offset, int my_offset)
{
	int mx = mx_offset + camera_get_offset().x;
	int my = my_offset + camera_get_offset().y;
	int tile_x = mx / 64;
	int tile_y = my / 64;
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
#include "simple_logger.h"
#include "level.h"
#include "gf2d_graphics.h"
#include "gfc_shape.h"
#include "camera.h"
#include "collider.h"
#include "enemy.h"

#include "player.h"

#include "spike.h"
#include "damagezone.h"
#include "speaker.h"
#include "tnt.h"
#include "wall.h"
#include "current.h"
#include "quest.h"
#include "eventer.h"

void level_tile_layer_build(Level* level) {
	if (!level) return;
	if (!level->tileSet)return;
	int i, j, index;
	GFC_Vector2D position;
	Uint32 frame;

	slog("level is real");


	if (level->tileLayer) {
		gf2d_sprite_free(level->tileLayer);
	}
	level->tileLayer = gf2d_sprite_new();

	level->tileLayer->surface = gf2d_graphics_create_surface(
		level->tileMapWidth * level->tileSet->frame_w,
		level->tileMapHeight * level->tileSet->frame_h
	);

	level->tileLayer->frame_w = level->tileMapWidth * level->tileSet->frame_w;
	level->tileLayer->frame_h = level->tileMapHeight * level->tileSet->frame_h;

	if (!level->tileLayer->surface) {
		slog("Failed to create surface!!!!");
		return;
	}

	for (j = 0; j < level->tileMapHeight; j++) {
		for (i = 0; i < level->tileMapWidth; i++) {
			index = i + (j * level->tileMapWidth);
			if (level->tileMap[index] == 0)continue;

			position.x = i * level->tileSet->frame_w;// *4;
			position.y = j * level->tileSet->frame_h;// *4;
			frame = level->tileMap[index] - 1;
			//GFC_Vector2D scalevar = gfc_vector2d(4, 4);
			//GFC_Vector2D* scale = &scalevar;

			gf2d_sprite_draw_to_surface(
				level->tileSet,
				position,
				NULL,//scale,
				NULL,
				frame,
				level->tileLayer->surface
			);
		}
	}
	level->tileLayer->texture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), level->tileLayer->surface);
	slog("finished making level texture");
	if (!level->tileLayer->texture) {
		slog("failed to load texture from level tile layer surface");
		return;
	}


}


void level_collision_layer_build(Level* level) {
	if (!level) return;
	if (!level->tileSet)return;
	int i, j, index;
	GFC_Vector2D position;

	slog("level is real");

	for (j = 0; j < level->tileMapHeight; j++) {
		for (i = 0; i < level->tileMapWidth; i++) {
			index = i + (j * level->tileMapWidth);
			if (level->tileMap[index] == 0)continue;
			position.x = i * level->tileSet->frame_w;// *4;
			position.y = j * level->tileSet->frame_h;
			GFC_Rect newRect = gfc_rect(position.x, position.y, level->tileSet->frame_w, level->tileSet->frame_h);
			Collider* newCollider = collider_new(newRect, false, NULL);
			//GFC_Vector2D scalevar = gfc_vector2d(4, 4);
			//GFC_Vector2D* scale = &scalevar;
		}
	}
	level->tileLayer->texture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), level->tileLayer->surface);
	slog("finished making level collisions");
}

SJson* entityJson(const char* type, GFC_Vector2D pos)
{
	SJson* entity_json = sj_object_new();
	SJson* entity = sj_new_str(type);
	SJson* pos_x = sj_new_float(pos.x);
	SJson* pos_y = sj_new_float(pos.y);


	sj_object_insert(entity_json, "entity", entity);
	sj_object_insert(entity_json, "pos_x", pos_x);
	sj_object_insert(entity_json, "pos_y", pos_y);
	return entity_json;
}

void level_save(Level* level)
{
	SJson* json = sj_object_new();
	SJson* ljson = sj_object_new();

	
	SJson* background = sj_new_str("images/backgrounds/screenshot.png");
	sj_object_insert(ljson, "background", background);
	SJson* tileSet = sj_new_str("images/tiles-big.png");
	sj_object_insert(ljson, "tileSet", tileSet);
	SJson* frame_w = sj_new_int(64);
	sj_object_insert(ljson, "frame_w", frame_w);
	SJson* frame_h = sj_new_int(64);
	sj_object_insert(ljson, "frame_h", frame_h);
	SJson* frames_per_line = sj_new_int(1);
	sj_object_insert(ljson, "frames_per_line", frames_per_line);

	slog("we made it to tilemap");
	SJson* tileMap = sj_array_new();
	for (int i = 0; i < level->tileMapHeight; i++)
	{
		SJson* tileMapRow = sj_array_new();
		for (int j = 0; j < level->tileMapWidth; j++)
		{
			Uint8 tile = level->tileMap[j + (i*level->tileMapWidth)];
			slog("doing tile %i", tile);
			SJson* mapValue = sj_new_uint8(tile);
			sj_array_append(tileMapRow, mapValue);
		}
		sj_array_append(tileMap, tileMapRow);
	}

	sj_object_insert(ljson, "tileMap", tileMap);
	slog("wrote tilemap");



	slog("we got before entities");
	


	SJson* entities = entities_for_json();
	sj_object_insert(ljson, "entities", entities);
	slog("we got before player spawn");

	

	SJson* player_spawn_x = sj_new_float(player_get()->position.x);
	SJson* player_spawn_y = sj_new_float(player_get()->position.y);
	sj_object_insert(ljson, "player_spawn_x", player_spawn_x);
	sj_object_insert(ljson, "player_spawn_y", player_spawn_y);

	slog("wrote the player spawn");
	sj_object_insert(json, "level", ljson);
	sj_save(json, "maps/editor_level.json");
	slog("wrote the json");
	sj_object_free(json);
}


Level* level_load(const char* filename)
{
	Level* level = NULL;
	if (!filename) {
		slog("level filename not foundededed");
		return NULL;
	}
	SJson* json = sj_load(filename);
	SJson* ljson = sj_load(filename);

	int width = 0, height = 0;

	SJson* vertical, * horizontal;

	int i, j;
	SJson* item;
	int tile;


	const char* tileSet;
	int frame_w, frame_h;
	int frames_per_line;

	if (!json)
	{
		slog("failed to load world file %s", filename);
		return NULL;
	}

	ljson = sj_object_get_value(json, "level");
	if (!ljson) {
		slog("no level in json %s", filename);
		sj_free(json);
		return NULL;
	}



	vertical = sj_object_get_value(ljson, "tileMap");
	if (!vertical) {
		slog("no tilemap in json %s", filename);
		sj_free(json);
		return NULL;
	}
	height = sj_array_get_count(vertical);
	horizontal = sj_array_get_nth(vertical, 0);
	width = sj_array_get_count(horizontal);

	level = level_new(width, height);

	if (!level) {
		slog("failed to create space for a new world for file %s", filename);
		sj_free(json);
		return NULL;
	}


	for (j = 0; j < height; j++) {
		horizontal = sj_array_get_nth(vertical, j);
		if (!horizontal)continue;
		for (i = 0; i < width; i++) {
			item = sj_array_get_nth(horizontal, i);
			if (!item)continue;
			tile = 0;
			sj_get_integer_value(item, &tile);
			level->tileMap[i + (j * width)] = tile;
		}
	}

	tileSet = sj_object_get_value_as_string(ljson, "tileSet");
	if (!tileSet)return NULL;
	level->tileSet = tileSet;

	sj_object_get_value_as_int(ljson, "frame_w", &frame_w);
	sj_object_get_value_as_int(ljson, "frame_h", &frame_h);
	sj_object_get_value_as_int(ljson, "frames_per_line", &frames_per_line);

	level->tileSet = gf2d_sprite_load_all(
		tileSet,
		frame_w,
		frame_h,
		frames_per_line,
		1
	);
	sj_object_get_value_as_int(ljson, "frame_w", &frame_w);

	level->background = gf2d_sprite_load_image(sj_object_get_value_as_string(ljson, "background"));

	level_tile_layer_build(level);
	level_collision_layer_build(level);
	//return level;
	
	SJson* entities = sj_object_get_value(ljson, "entities");
	for (int i = 0; i < sj_array_get_count(entities); i++)
	{
		SJson* entity = sj_array_get_nth(entities, i);
		float* entity_position_x = malloc(sizeof(float));
		float* entity_position_y = malloc(sizeof(float));
		const char* entity_name = malloc(sizeof(char[50]));
		sj_object_get_value_as_float(entity, "pos_x", entity_position_x);
		sj_object_get_value_as_float(entity, "pos_y", entity_position_y);
		entity_name = sj_object_get_value_as_string(entity, "entity");

		if (strcmp(entity_name, "walky") == 0)
		{
			if (entity_position_x != NULL && entity_position_y != NULL) {
				walky_enemy_entity_new(gfc_vector2d(*entity_position_x, *entity_position_y));
			}
		}
		else if (strcmp(entity_name, "turret") == 0)
		{
			if (entity_position_x != NULL && entity_position_y != NULL) {
				turret_enemy_entity_new(gfc_vector2d(*entity_position_x, *entity_position_y));
			}
		}
		else if (strcmp(entity_name, "floater") == 0)
		{
			if (entity_position_x != NULL && entity_position_y != NULL) {
				floater_enemy_entity_new(gfc_vector2d(*entity_position_x, *entity_position_y));
			}
		}
		else if (strcmp(entity_name, "swinger") == 0)
		{
			if (entity_position_x != NULL && entity_position_y != NULL) {
				enemy_entity_new(gfc_vector2d(*entity_position_x, *entity_position_y));
			}
		}
		else if (strcmp(entity_name, "bruiser") == 0)
		{
			if (entity_position_x != NULL && entity_position_y != NULL) {
				bruiser_enemy_entity_new(gfc_vector2d(*entity_position_x, *entity_position_y));
			}
		}

		//////NON ENEMIES//////
		else if (strcmp(entity_name, "spike") == 0)
		{
			if (entity_position_x != NULL && entity_position_y != NULL) {
				spike_entity_new(gfc_vector2d(*entity_position_x, *entity_position_y), 40, gfc_vector2d(64, 64));
			}
		}
		else if (strcmp(entity_name, "damagezone") == 0) {
			if (entity_position_x != NULL && entity_position_y != NULL) {
				damagezone_entity_new(gfc_vector2d(*entity_position_x, *entity_position_y), 40, gfc_vector2d(128, 128));
			}
		}
		else if (strcmp(entity_name, "speaker") == 0) {
			if (entity_position_x != NULL && entity_position_y != NULL) {
				speaker_entity_new(gfc_vector2d(*entity_position_x, *entity_position_y));
			}
		}
		else if (strcmp(entity_name, "tnt") == 0) {
			if (entity_position_x != NULL && entity_position_y != NULL) {
				tnt_entity_new(gfc_vector2d(*entity_position_x, *entity_position_y));
			}
		}
		else if (strcmp(entity_name, "wall_explosive") == 0) {
			if (entity_position_x != NULL && entity_position_y != NULL) {
				wall_entity_new(gfc_vector2d(*entity_position_x, *entity_position_y),2, gfc_vector2d(64, 128));
			}
		}
		else if (strcmp(entity_name, "wall_melee") == 0) {
			if (entity_position_x != NULL && entity_position_y != NULL) {
				wall_entity_new(gfc_vector2d(*entity_position_x, *entity_position_y), 1, gfc_vector2d(64, 128));
			}
		}
		else if (strcmp(entity_name, "current") == 0) {
			if (entity_position_x != NULL && entity_position_y != NULL) {
				current_entity_new(gfc_vector2d(*entity_position_x, *entity_position_y), gfc_vector2d(0, 1), gfc_vector2d(64, 128));
			}
		}
		else if (strcmp(entity_name, "quest_jump") == 0) {
			if (entity_position_x != NULL && entity_position_y != NULL) {
				quester_entity_new(gfc_vector2d(*entity_position_x, *entity_position_y), QUEST_JUMP);
			}
		}
		else if (strcmp(entity_name, "quest_floater") == 0) {
			if (entity_position_x != NULL && entity_position_y != NULL) {
				quester_entity_new(gfc_vector2d(*entity_position_x, *entity_position_y), QUEST_FLOATER);
			}
		}
		else if (strcmp(entity_name, "quest_talk") == 0) {
			if (entity_position_x != NULL && entity_position_y != NULL) {
				quester_entity_new(gfc_vector2d(*entity_position_x, *entity_position_y), QUEST_TALK);
			}
		}
		else if (strcmp(entity_name, "quest_talk_to") == 0) {
			if (entity_position_x != NULL && entity_position_y != NULL) {
				quester_entity_new(gfc_vector2d(*entity_position_x, *entity_position_y), -1);
			}
		}


		free(entity_position_x);
		free(entity_position_y);
		
	}
	
	slog("setting spawn");
	///////player spawn////////
	float* player_x = malloc(sizeof(float));
	float* player_y = malloc(sizeof(float));
	sj_object_get_value_as_float(ljson, "player_spawn_x", player_x);
	sj_object_get_value_as_float(ljson, "player_spawn_y", player_y);
	level->playerSpawn = gfc_vector2d(*player_x, *player_y);
	free(player_x);
	free(player_y);
	slog("set spawn");
	

	sj_free(json);
	return level;
}

Level* level_test_new() {
	int i, j;
	int width = 100, height = 80;
	Level* level;

	level = level_new(width, height);
	if (!level) return NULL;
	level->background = gf2d_sprite_load_image("images/backgrounds/screenshot.png");
	level->tileSet = gf2d_sprite_load_all("images/tiles.png", 16, 16, 1, true);

	for (i = 0; i < width; i++)
	{
		level->tileMap[i] = 1;
		level->tileMap[i + ((height - 1) * width)] = 1;
	}
	for (i = 0; i < height; i++)
	{
		level->tileMap[i*width] = 1;
		level->tileMap[i*width + ((width - 1))] = 1;
	}
	level_tile_layer_build(level);
	return level;

}

//Create a new level
Level* level_new(Uint32 width, Uint32 height) {
	Level* level;

	if ((!width) || (!height)) {

		slog("cannot make a world with zero width or height");
		return NULL;
	}
	level = gfc_allocate_array(sizeof(Level), 1);
	if (!level) {
		slog("failed to allocate level");
		return NULL;
	}
	//defaults go here
	level->tileMap = gfc_allocate_array(sizeof(Uint8), height * width);
	level->tileMapHeight = height;
	level->tileMapWidth = width;
	return level;
}

//Free a level
void level_free(Level* level) {
	if (!level)return;
	gf2d_sprite_free(level->background);
	gf2d_sprite_free(level->tileSet);
	gf2d_sprite_free(level->tileLayer);
	free(level->tileMap);
	free(level);
}

//0000000000000000000000

void level_draw(Level* level)
{
	if (!level) {
		slog("NO LEVEL IDIOT!!");
		return;
	}

	/*int i, j;
	int index;
	int frame;
	//probably set up scale here later
	GFC_Vector2D scaleStruct = gfc_vector2d(1, 1);
	GFC_Vector2D *scale = &scaleStruct;
	GFC_Vector2D position;*/
	GFC_Vector2D new_position;
	gfc_vector2d_add(new_position, gfc_vector2d(0, 0), camera_get_offset());
	GFC_Vector2D new_position_halfsies;
	gfc_vector2d_add(new_position_halfsies, gfc_vector2d(0, 0), gfc_vector2d_multiply(camera_get_offset(), gfc_vector2d(0.5,0.5)));
	gf2d_sprite_draw_image(level->background, new_position_halfsies);

	gf2d_sprite_draw_image(level->tileLayer, new_position);
	/*if (!level->tileSet)return;
	for (j = 0; j < level->tileMapHeight; j++) {
		for (i = 0; i < level->tileMapWidth; i++) {
			index = i + (j * level->tileMapWidth);
			if (level->tileMap[index] == 0)continue;
			position.x = i * level->tileSet->frame_w * (scale->x);
			position.y = j * level->tileSet->frame_h * (scale->y);
			frame = level->tileMap[index] - 1;
			gf2d_sprite_draw(
				level->tileSet,
				position,
				scale,
				NULL,
				NULL,
				NULL,
				NULL,
				frame
			);
		}
	}*/

	Uint8 tile;
	GFC_Vector2D offset;
}

void level_setup_camera_bounds(Level* level)
{
	if (!level) return;
	if (!level->tileLayer) return;
	slog("we are setting boundaries");
	camera_set_bounds(gfc_rect(0, 0, level->tileLayer->surface->w,level->tileLayer->surface->h));
	slog("finished doing that");
	camera_apply_bounds();
	camera_enable_binding(1);
	slog("finished level_setup_camera_bounds()");
}

level_refresh_tiles(Level* level)
{
	level_tile_layer_build(level);
	level_collision_layer_build(level);
}



/*eol@eof*/
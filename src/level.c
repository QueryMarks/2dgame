#include "simple_logger.h"
#include "level.h"
#include "gf2d_graphics.h"
#include "gfc_shape.h"
#include "camera.h"


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
	return level;
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


/*eol@eof*/
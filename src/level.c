#include "simple_logger.h"
#include "level.h"


Level* level_test_new() {
	int i, j;
	int width = 75, height = 45;
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
	slog("level is being drauwn");

	int i, j;
	int index;
	int frame;
	//probably set up scale here later
	GFC_Vector2D scaleStruct = gfc_vector2d(1, 1);
	GFC_Vector2D *scale = &scaleStruct;
	GFC_Vector2D position;
	slog("did we draw the background?");
	gf2d_sprite_draw_image(level->background, gfc_vector2d(0, 0));
	slog("we draw the background!");
	if (!level->tileSet)return;
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
	}

	Uint8 tile;
	GFC_Vector2D offset;
}

/*eol@eof*/
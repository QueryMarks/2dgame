#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "gf2d_sprite.h"

typedef struct {
	Sprite* background; /**<background image for the level */
	Sprite* tileLayer; /**<prerendered tile layer*/
	Sprite* tileSet;
	Uint8* tileMap;
	Uint32 tileMapWidth;
	Uint32 tileMapHeight;
}Level;

Level* level_test_new();

Level* level_load(const char* filename);

//Create a new level
Level *level_new();

//Free a level
void level_free(Level* level);

//Draw the level
void level_draw(Level* level);

//setup the camera bounds based on the level
void level_setup_camera_bounds(Level* level);



#endif
#ifndef __WINDOW_H__
#define __WINDOW_H__


#include "gfc_types.h"
#include "gf2d_sprite.h"
#include "gfc_shape.h"

#include "SDL_ttf.h"


/**
* @purpose This lets me use windows and menus and wenus
**/
typedef struct Window_S {
	Uint8 _inuse; /**<flag for keeping track of memory usage*/
	Sprite* windowSprite;
	Sprite* textSprite;
	//button goes here when we add it;
	GFC_Vector2D windowPosition;
	GFC_Vector2D textPosition;
	TTF_Font* font;
}Window;

void window_manager_init(Uint32 max);

void window_manager_close();

Window* window_new();

void window_free(Window* self);

void window_manager_think_all();

void window_manager_update_all();

void window_manager_draw_all();



void write_window_text(Window* self, const char* write);

#endif
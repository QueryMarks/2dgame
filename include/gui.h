#ifndef __GUI_H__
#define __GUI_H__

#include "SDL_ttf.h"
#include "gf2d_sprite.h"

static Sprite *guiSprite = { 0 };


void write_gui(const char* write);
void draw_gui();

#endif
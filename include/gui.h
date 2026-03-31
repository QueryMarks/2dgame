#ifndef __GUI_H__
#define __GUI_H__

#include "SDL_ttf.h"
#include "gf2d_sprite.h"

static Sprite *guiSprite = { 0 };

static Sprite* dialogueSprite = { 0 };
static Sprite* dialogueBoxSprite = { 0 };

static Sprite* choice1 = { 0 };
static Sprite* choice2 = { 0 };


typedef struct GUI_S {
	Sprite* guiSprite;
	Sprite* dialogueSprite;
	Sprite* dialogueBoxSprite;
	Sprite* choice1;
	Sprite* choice2;
}GUI;

void write_gui(const char* write);
void write_dialogue(const char* write);
void write_choices(const char* write, const char* write2);
void hide_dialogue();
void draw_gui();

#endif
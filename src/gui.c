#include "gui.h"
#include "gf2d_graphics.h"
#include "simple_logger.h"

#include "entity.h"
#include "player.h"

//much of this is taken using https://stackoverflow.com/questions/22886500/how-to-render-text-in-sdl2
//i will write something more elegant later this week when i have actual gui stuff to use



void write_gui(const char* write) {

	if (TTF_Init() != 0) {
		slog("something's wrong with the ttf diffuser");
		TTF_Quit();
		return;
	}

	TTF_Font* font = TTF_OpenFont("fonts/DejaVuSans.ttf", 24);
	SDL_Color writeColor = { 100, 200, 150 };

	//guiSprite->surface = gf2d_graphics_create_surface(300, 300);
	if (guiSprite != NULL) {
		gf2d_sprite_free(guiSprite);
	}
	guiSprite = gf2d_sprite_new();
	guiSprite->surface = TTF_RenderText_Solid(font, write, writeColor);
	if (guiSprite->surface == NULL) {
		slog("the surface didn't work");
	}
	// = surfaceMessage;
	guiSprite->texture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), guiSprite->surface);
	if (guiSprite->texture == NULL) {
		slog("the texture didn't work");
	}
	// = guiTexture;

	guiSprite->frame_w = 300;
	guiSprite->frame_h = 120;
	guiSprite->frames_per_line = 1;

	TTF_Quit();
}

void draw_gui() {
	gf2d_sprite_draw_image(guiSprite, gfc_vector2d(0,0));
}


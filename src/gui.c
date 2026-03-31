#include "gui.h"
#include "gf2d_graphics.h"
#include "simple_logger.h"

#include "entity.h"
#include "player.h"

//much of this is taken using https://stackoverflow.com/questions/22886500/how-to-render-text-in-sdl2
//i will write something more elegant later this week when i have actual gui stuff to use

Uint8 dialogue_up = 0;

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

void write_dialogue(const char* write) {
	if (TTF_Init() != 0) {
		slog("something's wrong with the ttf diffuser");
		TTF_Quit();
		return;
	}

	TTF_Font* font = TTF_OpenFont("fonts/DejaVuSans.ttf", 12);
	SDL_Color writeColor = { 150, 100, 100 };

	//dialogueSprite->surface = gf2d_graphics_create_surface(300, 300);
	if (dialogueSprite != NULL) {
		gf2d_sprite_free(dialogueSprite);
	}
	dialogueSprite = gf2d_sprite_new();
	dialogueSprite->surface = TTF_RenderText_Solid(font, write, writeColor);
	if (dialogueSprite->surface == NULL) {
		slog("the surface didn't work");
	}
	// = surfaceMessage;
	dialogueSprite->texture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), dialogueSprite->surface);
	if (dialogueSprite->texture == NULL) {
		slog("the texture didn't work");
	}
	// = guiTexture;

	dialogueSprite->frame_w = 64*10;
	dialogueSprite->frame_h = 48;
	dialogueSprite->frames_per_line = 1;

	dialogueBoxSprite = gf2d_sprite_load_all("images/dialoguebox.png", 950, 256, 1, 1);
	SDL_SetTextureAlphaMod(dialogueBoxSprite->texture, 100);

	dialogue_up = 1;
	TTF_Quit();
}

void write_choices(const char* write, const char* write2) {
	if (TTF_Init() != 0) {
		slog("something's wrong with the ttf diffuser");
		TTF_Quit();
		return;
	}

	TTF_Font* font = TTF_OpenFont("fonts/DejaVuSans.ttf", 12);
	SDL_Color writeColor = { 150, 100, 100 };

	//choice1->surface = gf2d_graphics_create_surface(300, 300);
	if (choice1 != NULL) {
		gf2d_sprite_free(choice1);
	}
	choice1 = gf2d_sprite_new();
	choice1->surface = TTF_RenderText_Solid(font, write, writeColor);
	if (choice1->surface == NULL) {
		slog("the surface didn't work");
	}
	// = surfaceMessage;
	choice1->texture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), choice1->surface);
	if (choice1->texture == NULL) {
		slog("the texture didn't work");
	}


	choice1->frame_w = 64 * 5;
	choice1->frame_h = 64;
	choice1->frames_per_line = 1;


	/////CHOICE 2
	/////////////

	if (choice2 != NULL) {
		gf2d_sprite_free(choice2);
	}
	choice2 = gf2d_sprite_new();
	choice2->surface = TTF_RenderText_Solid(font, write2, writeColor);
	if (choice2->surface == NULL) {
		slog("the surface didn't work");
	}
	// = surfaceMessage;
	choice2->texture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), choice2->surface);
	if (choice2->texture == NULL) {
		slog("the texture didn't work");
	}
	// = guiTexture;

	choice2->frame_w = 64 * 5;
	choice2->frame_h = 64;
	choice2->frames_per_line = 1;
	dialogue_up = 2;
	TTF_Quit();
}

void hide_dialogue() {
	dialogue_up = 0;
}

void draw_gui() {
	gf2d_sprite_draw_image(guiSprite, gfc_vector2d(0,0));
	if (dialogue_up >= 1)
	{
		gf2d_sprite_draw_image(dialogueBoxSprite, gfc_vector2d(64, 8 * 64));
		gf2d_sprite_draw_image(dialogueSprite, gfc_vector2d(64+32, 8*64+32));
		if (dialogue_up == 2)
		{
			gf2d_sprite_draw_image(choice1, gfc_vector2d(64, 10 * 64));
			gf2d_sprite_draw_image(choice2, gfc_vector2d(650, 10*64));

		}
	}
	
}


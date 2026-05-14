#include "element.h"
#include "gfc_input.h"
#include "gf2d_graphics.h"

#include "SDL_ttf.h"

#include "game.h"

void element_think(struct Element_S* self);
void* button_update(struct Element_S* self);
void* text_update(struct Element_S* self);
void onclick_close(struct Element_S* self);

Element* element_new(Window *window) {
	int i;
	if (!window->elements) {
		slog("this window has not initialized its elements list i suppose");
		return NULL;
	}
	for (i = 0; i < 32; i++) {
		if (window->elements[i]._inuse == 1) continue;
		window->elements[i]._inuse = 1;
		window->elements[i].position = gfc_vector2d(300, 50);
		window->elements[i].sprite = gf2d_sprite_load_all("images/speaker.png", 64, 64, 1, 0);
		window->elements[i].window = window;
		window->elements[i].elementType = ET_BUTTON;
		window->elements[i].bounds = gfc_rect(window->elements[i].position.x+window->position.x, window->elements[i].position.y+window->position.y, 64, 64);


		window->elements[i].update = button_update;
		window->elements[i].onclick = onclick_close;
		window->elements[i].frame = 0;

		return &window->elements[i];
	}
}


Element* button_new(
	Window* window,
	GFC_Vector2D position,
	Sprite* sprite,
	GFC_Vector2D size,
	void* onclick)
{
	int i;
	if (!window->elements) {
		slog("this window has not initialized its elements list i suppose");
		return NULL;
	}
	for (i = 0; i < 32; i++) {
		if (window->elements[i]._inuse == 1) continue;
		window->elements[i]._inuse = 1;
		window->elements[i].position = position;
		if (sprite == NULL) {
			window->elements[i].sprite = gf2d_sprite_load_all("images/speaker.png", 64, 64, 1, 0);
		}
		else
		{
			window->elements[i].sprite = sprite;
		}

		window->elements[i].window = window;
		window->elements[i].elementType = ET_BUTTON;
		window->elements[i].bounds = gfc_rect(window->elements[i].position.x + window->position.x, window->elements[i].position.y + window->position.y, 64, 64);

		window->elements[i].frame = 0;
		window->elements[i].update = button_update;
		window->elements[i].onclick = onclick;
		return &window->elements[i];
	}
}

Element* text_new(
	Window* window,
	GFC_Vector2D position,
	GFC_Vector2D size,
	const char* text
	)
{
	int i;
	if (!window->elements) {
		slog("this window has not initialized its elements list i suppose");
		return NULL;
	}
	for (i = 0; i < 32; i++) {
		if (window->elements[i]._inuse == 1) continue;
		window->elements[i]._inuse = 1;
		window->elements[i].position = position;
		

		window->elements[i].window = window;
		window->elements[i].elementType = ET_TEXT;
		window->elements[i].bounds = gfc_rect(window->elements[i].position.x + window->position.x, window->elements[i].position.y + window->position.y, 64, 64);


		window->elements[i].update = text_update;
		
		slog("made it to ttf init");
		
		if (TTF_Init() != 0) {
			slog("something's wrong with the ttf diffuser");
			TTF_Quit();
			return;
		}

		TTF_Font* font = TTF_OpenFont("fonts/DejaVuSans.ttf", 12);
		SDL_Color writeColor = { 250, 200, 200 };

		Sprite* mySprite = gf2d_sprite_new();
		mySprite->surface = TTF_RenderText_Solid(font, text, writeColor);
		if (mySprite->surface == NULL) {
			slog("the surface didn't work");
		}
		// = surfaceMessage;
		mySprite->texture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), mySprite->surface);
		if (mySprite->texture == NULL) {
			slog("the texture didn't work");
		}

		mySprite->frame_w = size.x;
		mySprite->frame_h = size.y;
		mySprite->frames_per_line = 1;
		window->elements[i].sprite = mySprite;
		window->elements[i].frame = 0;


		return &window->elements[i];
	}
}


void element_think(struct Element_S* self) {
	return;
}
void* button_update(struct Element_S* self) {
	//slog("i am SOOOOOOO button updating");
	if (gfc_input_key_pressed("b")) {
		int mx, my;
		SDL_GetMouseState(&mx, &my);
		if (gfc_point_in_rect(gfc_vector2d(mx, my), self->bounds))
		{
			self->onclick(self);
		}
	}
}

void* text_update(struct Element_S* self)
{

}

void element_free(struct Element_S* self) {
	if (self->sprite)gf2d_sprite_free(self->sprite);
	self->_inuse = false;
}


void element_draw(Element* self) {
	if (!self)return;
	GFC_Vector2D drawpos;
	gfc_vector2d_add(drawpos, self->position, self->window->position);
	gf2d_sprite_draw(self->sprite, drawpos,NULL,NULL,NULL,NULL,NULL, (Uint32)self->frame);
};

void onclick_close(struct Element_S* self)
{
	self->window->hidden = 1;
	
}

void onclick_start_level(struct Element_S* self)
{
	game_title_exit();
}

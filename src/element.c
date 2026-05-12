#include "element.h"
#include "gfc_input.h"

#include "game.h"

void element_think(struct Element_S* self);
void* button_update(struct Element_S* self);
void* element_free(struct Element_S* self);
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


		window->elements[i].update = button_update;
		window->elements[i].onclick = onclick;
	}
}

void element_think(struct Element_S* self) {
	return;
}
void* button_update(struct Element_S* self) {
	//slog("i am SOOOOOOO button updating");
	if (gfc_input_key_pressed("1")) {
		self->onclick(self);
	}
	return;
}
void* element_free(struct Element_S* self) {
	if (self->sprite)gf2d_sprite_free(self->sprite);
	self->_inuse = false;
}


void element_draw(Element* self) {
	if (!self)return;
	GFC_Vector2D drawpos;
	gfc_vector2d_add(drawpos, self->position, self->window->position);
	gf2d_sprite_draw_image(self->sprite, drawpos);
};

void onclick_close(struct Element_S* self)
{
	int mx, my;
	SDL_GetMouseState(&mx, &my);
	if (gfc_point_in_rect(gfc_vector2d(mx, my), self->bounds)) {
		self->window->hidden = 1;
	}
	
}

void onclick_start_level(struct Element_S* self)
{
	int mx, my;
	SDL_GetMouseState(&mx, &my);
	if (gfc_point_in_rect(gfc_vector2d(mx, my), self->bounds)) {
		game_title_exit();
	}
}

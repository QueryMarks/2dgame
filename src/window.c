#include "simple_logger.h"

#include "gfc_types.h"
#include "gf2d_sprite.h"

#include "window.h"
#include "element.h"



typedef struct
{
	Sprite* background;
	Window* windowList;
	Uint32 windowMax;
}WindowManager;


static WindowManager windowManager = { 0 };


void window_manager_init(Uint32 max)
{

	slog("initializing window system");
	if (!max) {
		slog("no window max for manager");
		return;
	}
	windowManager.windowList = malloc(sizeof(Window) * max);

	if (!windowManager.windowList) {
		slog("failed to allocate %i windows", max);
		return;
	}

	windowManager.windowMax = max;
	windowManager.background = gf2d_sprite_load_all("images/dialoguebox.png", 950, 256, 1, 1);
	SDL_SetTextureAlphaMod(windowManager.background->texture, 100);
	atexit(window_manager_close);
	slog("initialized window system");
}


void window_manager_close()
{
	int i;
	if (!windowManager.windowList)return;

	for (i = 0; i < windowManager.windowMax; i++) 
	{
		window_free(&windowManager.windowList[i]);
	}
	free(windowManager.windowList);
	memset(&windowManager, 0, sizeof(WindowManager));
	slog("closed window system");
}

Window* window_new() {
	int i;
	if (!windowManager.windowList) {
		slog("window system has not yet been initialized");
		return NULL;
	}
	for (i = 0; i < windowManager.windowMax; i++) {
		if (windowManager.windowList[i]._inuse == 1)continue;
		windowManager.windowList[i]._inuse = 1;
		windowManager.windowList[i].position = gfc_vector2d(200, 200);
		windowManager.windowList[i].sprite = windowManager.background;
		windowManager.windowList[i].hidden = true;

		return &windowManager.windowList[i];
	}
	return NULL;
}

void window_draw(Window* self) {
	if (!self)return;
	if (!self->hidden) {
		gf2d_sprite_draw_image(self->sprite, gfc_vector2d(200, 200));
	}
	//slog("about to draw in window_draw");
	
	
}

void window_manager_draw_all() {
	//slog("drawing windows");
	int i;
	if (!windowManager.windowList) {
		//slog("no window list to draw");
		return;
	}
	for (i = 0; i < windowManager.windowMax; i++) {
		//slog("drawing window %i", i);
		if (windowManager.windowList[i]._inuse == 1) {
			if (windowManager.windowList[i].sprite != NULL)
			{
				//slog("window_draw about to get called");
				window_draw(&windowManager.windowList[i]);
				//slog("drew this window fully");
			}
			
		}
	}
}
void window_free(Window* self) {
	if(!self)return;
	if (self->sprite)gf2d_sprite_free(self->sprite);
}
#include "simple_logger.h"

#include "gfc_types.h"
#include "gf2d_sprite.h"

#include "window.h"



typedef struct
{
	Window* windowList;
	Uint32 windowMax;
}WindowManager;


static WindowManager windowManager = { 0 };


void window_manager_init(Uint32 max)

{
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
		if (windowManager.windowList[i]._inuse = 1)continue;
		windowManager.windowList[i]._inuse = 1;
		//window deets are set up after creation, not here.
		return &windowManager.windowList[i];
	}
	return NULL;
}

void window_draw(Window* self) {
	if (!self)return;
	if (self->windowSprite) {
		gf2d_sprite_draw_sprite(self->windowSprite, self->windowPosition);
	}
}
#include "simple_logger.h"
#include "camera.h"

static Camera camera = { 0 };



void camera_set_bounds(GFC_Rect_Bounds);



/*void level_setup_camera_bounds(Level* level)
{
	if (!level) return;
	
	camera_set_bounds(0,0,level->tileWidth* level->width,
	level->tileHeight* level->height);

}*/

/*void camera_snap_to_bounds()
{
	if (camera.view.x < camera.bounds.x)camera.view.x = camera.bounds.x;

}*/
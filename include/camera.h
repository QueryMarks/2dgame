#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "gfc_shape.h"

#include "entity.h"
#include "level.h"

typedef struct Camera_S{
	GFC_Vector2D position;
	GFC_Vector2D size;
	GFC_Rect bounds;
	Bool bindCamera;
}Camera;


GFC_Vector2D camera_get_position();

GFC_Vector2D camera_get_offset();

void camera_set_position(GFC_Vector2D position);

void camera_set_bounds(GFC_Rect bounds);

void camera_enable_binding(Bool bindCamera);

void camera_set_size(GFC_Vector2D size);
void camera_apply_bounds();
void camera_center_on(GFC_Vector2D target);

//void level_setup_camera_bounds(Level* level);

#endif
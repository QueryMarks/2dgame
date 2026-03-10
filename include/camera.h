#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "entity.h"
#include "level.h"

typedef struct Camera_S
{
	Uint8		_inuse;		/**<this is the flag for keeping track of memory usage*/
	Sprite* sprite;	/**<the entity's graphic, if it has one*/
	float		frame;		/**<the current frame of animation for the sprite*/
	GFC_Vector2D	position;	/**<where on the screen to draw the thing*/
	GFC_Vector2D	scale;
	void (*think)(struct Entity_S* self);		/**<think function called before update function for making decisions based on world state etc*/
	void (*update)(struct Entity_S* self);		/**<update function called after think function for acting on decisions made in think and result of interactions etc*/
	void (*free)(struct Entity_S* self);		/**<clean up any custom allocated data*/
	void* data;									/**<for ad hoc addition data for the entity*/
}Camera;

//void level_setup_camera_bounds(Level* level);

#endif
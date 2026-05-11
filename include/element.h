#ifndef __ELEMENT_H__
#define __ELEMENT_H__


#include "simple_logger.h"
#include "gfc_types.h"


#include "gf2d_sprite.h"
#include "window.h"

typedef enum
{
	ELEMENT_IDLE,
	ELEMENT_DISABLE,
	ELEMENT_HIGHLIGHT,
	ELEMENT_HIDDEN,
	ELEMENT_ACTIVE
}ElementState;


typedef enum
{
	ET_LABEL,
	ET_BUTTON
}ElementTypes;


struct Element_S
{
	int index;
	GFC_TextLine name;
	Sprite* sprite;
	Uint8 canHasFocus;
	Uint8 hasFocus;
	GFC_Rect bounds;
	int state;

	void(*think)(struct Element_S* self);
	void(*update)(struct Element_S* self);
	void(*free)(struct Element_S* self);
	void(*draw)(struct Element_S* self);
	Window window;
	void* data;
};




#endif
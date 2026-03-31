#ifndef __ENTITY_H__
#define __ENTITY_H__


#include "gfc_types.h"
#include "gf2d_sprite.h"

/**
* @purpose writing this to create the entity system
**/

typedef struct Entity_S
{
	Uint8		_inuse;		/**<this is the flag for keeping track of memory usage*/
	Sprite		*sprite;	/**<the entity's graphic, if it has one*/
	float		frame;		/**<the current frame of animation for the sprite*/
	GFC_Vector2D	position;	/**<where on the screen to draw the thing*/
	GFC_Vector2D	velocity;	/**<velocity of the entity*/
	GFC_Vector2D	scale;
	Uint8		removeme;
	int facing; //-1 is left, 1 is right
	Uint8 damager; //if 0, does not do damage. 1 = shot damage, 2 = contact damage
	Uint8 iframes;
	Uint8 solid; //0 if not solid, 1 if solid
	float health;
	void (*think)(struct Entity_S *self);		/**<think function called before update function for making decisions based on world state etc*/
	void (*update)(struct Entity_S* self);		/**<update function called after think function for acting on decisions made in think and result of interactions etc*/
	void (*free)(struct Entity_S* self);		/**<clean up any custom allocated data*/
	void (*collide)(struct Entity_S* self, void* collider);			/**<collide with another collider. because i am so poor at this we gotta cast the void pointer as a collider to avoid circular references in these header files*/
	void (*postupdate)(struct Entity_S* self);
	void *data;									/**<for ad hoc addition data for the entity*/
	void* collider;
}Entity;

/**
 *	@brief Initializes the entity management system.
 *	@param max the maximum number of entities that can exist at the same time
 * 
 */
void entity_manager_init(Uint32 max);


/**
* @brief clean up all active entities
* ignore do not clean up this entity
*/
void entity_manager_close();


/**
*	@brief get a blank entity for use
*	@return NULL on no more room or error, a blank entity otherwise
*/
Entity* entity_new();


/**
* @brief clean up an entity, and free its spot for future use
* @param self the entity to free
*/
void entity_free(Entity* self);


/**
* @brief run the think functions for all active entities
*/
void entity_manager_think_all();

/**
* @brief run the update functions for all active entities
*/
void entity_manager_update_all();

/**
* @brief run the draw functions for all active entities
*/
void entity_manager_draw_all();


#endif
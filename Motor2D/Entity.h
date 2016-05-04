#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Textures.h"
#include "Render.h"
#include "Map.h"
#include "App.h"
#include "Animation.h"
#include "Audio.h"
#include "Collision.h"
#include "GameManager.h"
#include "SDL\include\SDL.h"

#define TIME_TO_CHECK 100.0f

enum STATE
{
	IDLE,
	MOVE,
	MOVE_ALERT,
	MOVE_ALERT_TO_ATTACK,
	ATTACK,
	DYING,
	WAITING_PATH_MOVE,
	WAITING_PATH_MOVE_ALERT,
	WAITING_PATH_MOVE_ALERT_TO_ATTACK,
	
	// TANK STATES
	SIEGE_MODE_ON,
	SIEGE_MODE_OFF,
	IDLE_SIEGE_MODE,
	ATTACK_SIEGE_MODE
};

class Entity
{

public:

	// Positions and information
	fPoint			pos;						// World position of Entity. Upper_left corner.
	fPoint			center;						// World positoin of Entity. Center
	iPoint			tile_pos;					// Map position (tiles) of Entity
	uint			id;							// Identifier for Entity Manager
	Vector2D<int>   direction;					// Where is the entity looking at?
	float			angle;						// Looking right means 0 degrees, increasing counter-clock wise
	bool			to_delete;					// Entity Manager will delete this entity if it's true

	// Collider
	Collider*       coll;
	iPoint			collider_offset;			// Useful to correctly place the collider rect

	// Characterization and behaviour
	FACTION			faction;					// Player or computer
	ENTITY_TYPE		type;						// Unit or building	
	SPECIALIZATION  specialization;				// Marines, zerglings, bunkers, ...
	STATE			state;						// Idle, attack, move, ...

	// Graphics
	SDL_Texture		*tex;
	Animation		*current_animation;
	int		     	tex_width, tex_height;		// Dimensions of the sections of the frames

	// UI paramters
	SDL_Rect        selection_type;				// Section of the texture that contains circle selections
	iPoint		    circle_selection_offset;    // Useful to correctly place the circle selection
	iPoint			offset_life;				// Useful to correctly place the life's bar
	
	// Lifes attributes
	uint			max_hp;						// Max life value of this entity
	int             current_hp;					// The current life of the entity
	uint            max_hp_bars;				// How many green bars that current_hp represents?

	// Attack values and properties
	Entity			*target_to_attack = NULL;		// Which entity to attack
	float			damage;							// Value of its weapons
	Timer			timer_attack;					// To check time between attacks
	float			attack_frequency;				// How many miliseconds will wait to attack again?
	int				range_of_vision;				// How far can this entity detect another entity?
	int				range_to_attack;				// How far can this entity attack another entity?

	Timer			timer_to_check;					// Time between IA checking
	float			time_to_die;
	
	// Constructors
	Entity()
	{
		to_delete = false;
		timer_to_check.start();
		timer_attack.start();	
		state = IDLE;
		angle = 0.0f;
		direction.setToZero();
	};

	// Destructor
	~Entity()
	{
		SDL_DestroyTexture(tex);
	}

	virtual void setAnimationFromDirection()
	{

	}

	virtual bool update(float dt)
	{
 		return true;
	}

	virtual Entity* searchNearestEnemy()
	{
		return app->entity_manager->searchNearestEntityInRange(this);
	}

	virtual Entity* searchNearestAlly()
	{
		return app->entity_manager->searchNearestEntityInRange(this, true);
	}

	virtual list<Entity*> searchEntitiesInRange(float area_range)
	{
		return app->entity_manager->searchEntitiesInRange(target_to_attack, true, area_range);
	}

	virtual Entity* searchEnemy()
	{
		return app->entity_manager->searchEnemyToAttack(this);
	}

	virtual void draw()
	{
		app->render->blit(tex, pos.x, pos.y, &(current_animation->getCurrentFrame()));
	}

};

#endif !__ENTITY_H__
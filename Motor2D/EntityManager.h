#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "Module.h"
#include "Vector2D.h"
#include "SDL\include\SDL_rect.h"
#include "SDL\include\SDL_render.h"
#include <map>
#include <vector>

enum ENTITY_TYPE
{
	// Units
	MARINE,
	ZERGLING,
	// Buildings
	COMMANDCENTER
};

enum FACTION
{
	PLAYER,
	COMPUTER
};


class Entity;
using namespace std;

class EntityManager : public Module
{

public:

	EntityManager();

	// Destructor
	virtual ~EntityManager();

	// Called before render is available
	bool awake(pugi::xml_node&);

	// Called before the first frame
	bool start();

	// Called each loop iteration
	bool preUpdate();

	// Called each loop iteration
	bool update(float dt);

	// Called each loop iteration
	bool postUpdate();

	// Called before quitting
	bool cleanUp();

	Entity* const addEntity(iPoint &pos, ENTITY_TYPE type);
	Entity* const addInEnemyContainer(Entity* e);
	Entity* getEntity(uint id);
	
	void deleteEntity(map<uint, Entity*> selection);
	void deleteEntityKilled(Entity* e);
	void deleteAllEntities();
	uint getWaveZerglingSize();

	void AddEntityToWave(uint n, Entity* e);

	/*---------Methods for attacking other units------------*/

	//Method that Kills entities Used for the kill of zerglings and other units
	void KillEntity(map<uint, Entity*> selection);
	void KillEntity(Entity* e);
	Entity* whichEntityOnMouse();
	map<uint, Entity*>                     selection;

private:

	map<uint, Entity*>               active_entities;
	vector<Entity*>					   waveZerglings;
	//map<uint, Entity*>                     selection;
	map<uint, Entity*>						ToDelete;
	map<uint, Entity*>						enemyWave;
	uint next_ID;
	uchar filter;
	

	bool debug = false;

	//ROF
	Entity* marine;
	float angle;

	SDL_Rect			 selector;
	bool			selector_init;
	iPoint   initial_selector_pos;
	iPoint     final_selector_pos;

	void	  calculateSelector();

	//Textures
	SDL_Texture* circle_characters;
	SDL_Texture* hp_tex;
};

#endif // __EntityManager_H__

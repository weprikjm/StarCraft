#ifndef __TANK_H__
#define __TANK_H__

#include "Unit.h"

class Tank : public Unit
{
public:

	// IDLE animations
	Animation	idle_up;
	Animation	idle_right_up;
	Animation   idle_right;
	Animation	idle_right_down;
	Animation	idle_down;
	Animation	idle_left_down;
	Animation	idle_left;
	Animation	idle_left_up;
	vector<Animation*>   idle_animation_pack;

	// MOVING animations
	Animation	walk_up;
	Animation   walk_right_up;
	Animation   walk_right;
	Animation   walk_right_down;
	Animation   walk_down;
	Animation   walk_left_down;
	Animation   walk_left;
	Animation   walk_left_up;
	vector<Animation*>   move_animation_pack;

	// SIEGE MODE animation
	Animation   siege_mode_on;

	// SIEGE MODE animation
	Animation   siege_mode_off;

	// ATTACK animation
	Animation	attack_up;
	Animation	attack_right_up;
	Animation	attack_right;
	Animation	attack_right_down;
	Animation	attack_down;
	Animation	attack_left_down;
	Animation	attack_left;
	Animation	attack_left_up;
	vector<Animation*>   attack_animation_pack;

	// Dead animation
	Animation	dead;

	bool		siege_mode = false;

	Tank(iPoint &p);

	void move(float dt);
	void setAnimationFromDirection();

};

#endif __MARINE_H__
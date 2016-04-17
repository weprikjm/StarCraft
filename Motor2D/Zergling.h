#ifndef __ZERGLING_H__
#define __ZERGLING_H__

#include "Unit.h"

class Zergling : public Unit
{
public:

	Animation idle_up;
	Animation idle_right_up;
	Animation idle_right;
	Animation idle_right_down;
	Animation idle_down;
	Animation idle_left_down;
	Animation idle_left;
	Animation idle_left_up;

	Animation walk_up;
	Animation walk_right_up;
	Animation walk_right;
	Animation walk_right_down;
	Animation walk_down;
	Animation walk_left_down;
	Animation walk_left;
	Animation walk_left_up;

	Animation attack_up;
	Animation attack_right_up;
	Animation attack_right;
	Animation attack_right_down;
	Animation attack_down;
	Animation attack_left_down;
	Animation attack_left;
	Animation attack_left_up;

	Animation dead;

	

	Zergling(iPoint &p)
	{
		// Positions and dimensions
		center = { (float)p.x, (float)p.y };

		tex_width = tex_height = 64;
		collider_offset = { -12, -14 };
		pos = { (float)p.x - (tex_width / 2), (float)p.y - (tex_height / 2) };
		tile_pos = app->map->worldToMap(app->map->data.back(), center.x, center.y);

		// Animation
		tex = app->tex->loadTexture("Units/New_Zergling64.png");
		#include"Zergling_animations.h";
		current_animation = &idle_down;

		// Collider
		coll = app->collision->addCollider({ center.x + collider_offset.x, center.y + collider_offset.y, 24, 26 }, COLLIDER_UNIT, app->entity_manager);

		// Another stuff
		type = UNIT;
		specialization = ZERGLING;
		state = IDLE;
		faction = COMPUTER;
		max_hp = 40;
		current_hp = 40.0f;
		max_hp_bars = 6;
		selection_type = { 3, 4, 22, 13 };
		circle_selection_offset = { 0, -1 };
		offset_life = { -19, 16 };
		flying = false;

		speed = 8.0f;
		range_to_view = 200;
		range_to_attack = 32;
		damage = 3.0f;
		attack_delay = 200.0f;
		time_to_die = 500.0f;
	}

	void checkAngle()
	{
		if (state == DYING)
		{
			current_animation = &dead;
		}
		else
		{
			if (angle > 360)
			{
				angle -= 360.f;
			}

			if (has_target)
			{
				// From 0 to 180 degrees
				if (angle >= 0.f && angle < 22.5f)
				{
					current_animation = &walk_up;
				}

				if (angle >= 45.f && angle < 67.5f)
				{
					current_animation = &walk_right_up;
				}

				if (angle >= 90.f && angle < 112.5f)
				{
					current_animation = &walk_right;
				}

				if (angle >= 135.f && angle < 157.5f)
				{
					current_animation = &walk_right_down;
				}

				// From 180 to 360 degrees
				if (angle >= 180.f && angle < 202.5f)
				{
					current_animation = &walk_down;
				}

				if (angle >= 225.f && angle < 247.5f)
				{
					current_animation = &walk_left_down;
				}

				if (angle >= 270.f && angle < 292.5f)
				{
					current_animation = &walk_left;
				}

				if (angle >= 315.f && angle < 337.5f)
				{
					current_animation = &walk_left_up;
				}

			}
			else
			{
				if (angle >= 0.f && angle < 22.5f)
				{
					if (state == ATTACK)
						current_animation = &attack_up;
					else
						current_animation = &idle_up;
				}

				else if (angle >= 45.f && angle < 67.5f)
				{
					if (state == ATTACK)
						current_animation = &attack_right_up;
					else
						current_animation = &idle_right_up;
				}
				else if (angle >= 90.f && angle < 112.5f)
				{
					if (state == ATTACK)
						current_animation = &attack_right;
					else
						current_animation = &idle_right;
				}
				else if (angle >= 135.f && angle < 157.5f)
				{
					if (state == ATTACK)
						current_animation = &attack_right_down;
					else
						current_animation = &idle_right_down;
				}
				else if (angle >= 180.f && angle < 202.5f)
				{
					if (state == ATTACK)
						current_animation = &attack_down;
					else
						current_animation = &idle_down;
				}
				else if (angle >= 225.f && angle < 247.5f)
				{
					if (state == ATTACK)
						current_animation = &attack_left_down;
					else
						current_animation = &idle_left_down;
				}
				else if (angle >= 270.f && angle < 292.5f)
				{
					if (state == ATTACK)
						current_animation = &attack_left;
					else
						current_animation = &idle_left;
				}
				else if (angle >= 315.f && angle < 337.5f)
				{
					if (state == ATTACK)
						current_animation = &attack_left_up;
					else
						current_animation = &idle_left_up;
				}
		}
		}
	}

	bool update(float dt)
	{
		checkAngle();   // This sets animation according to their angle direction
		coll->setPos(center.x + collider_offset.x, center.y + collider_offset.y);

		switch (state)
		{
		case IDLE:
			if ((timer_to_check += dt) >= TIME_TO_CHECK)
			{
				app->entity_manager->SetEnemyToAttackCommandCenter(this);
				timer_to_check = 0.0f;
			}
			break;
		case MOVE:
			if (has_target) move(dt);
			break;
		case MOVE_ALERT:
			if ((timer_to_check += dt) >= TIME_TO_CHECK)
			{
				if (searchNearestEnemy())
					LOG("Enemy found");
				timer_to_check = 0.0f;
			}
			if (has_target) move(dt);
			break;
		case ATTACK:
			if ((timer_attack_delay += dt) >= attack_delay)
			{
				attack();
				checkUnitDirection();
				timer_attack_delay = 0.0f;
			}
			break;
		case DYING:
			if ((timer_to_check += dt) >= time_to_die)
			{
				to_delete = true;
				coll->to_delete = true;
			}
			break;
		}
		return true;
	}

	void draw()
	{
		app->render->blit(tex, pos.x, pos.y, &(current_animation->getCurrentFrame()));
	}
};

#endif !__ZERGLING_H__
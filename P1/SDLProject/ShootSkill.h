#pragma once
#include "Entity.h"

class ShootSkillP :public Skill {
	ShootSkillP(float cd, Projectile* projectile) :Skill(cd) {}
	Projectile* activate(float speed, float power, float health, float duration, float x, float y, GLuint texture) {
		Projectile* proj = new Projectile(speed, power, health, duration, x, y, 0, texture);
	}
};
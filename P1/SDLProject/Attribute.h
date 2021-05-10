#pragma once
#include <vector>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
class Location {
private:
	glm::vec3 location;
	float r;
public:
	Location(float x, float y, float r);
	float getR() const;
	void spin(const float r,float);
	float getX() const;
	float getY() const;
	glm::vec3 getLocation() const {
		return location;
	}
	void setLocation(float x, float y, float r) {
		this->location = glm::vec3(x, y, r);
	}
	void move(glm::vec3,float);
};




class Status {
private:
	float speed;
	float power;
	float health;
public:
	//Status() {};
	Status(float speed):speed(speed),power(1),health(1) {
	
	};
	Status(float speed, float power, float health) :speed(speed), power(power), health(health) {
		
	}

	void buffed(float spd, float pow, float hp) {
		speed *= spd;
		power *= pow;
		health *= hp;
	}


	void setStatus(float spd, float pow, float hp) {
		speed = spd;
		power = pow;
		health = hp;
	}

	bool loseLife(float dmg) {
		if (health <= dmg) {
			health = 0;
			return true;
		}
		health -= dmg;
		return false;
	}

	float getPower() const {
		return power;
	}
	
	float getHealth() const {
		return health;
	}

	float getSpeed() const {
		return speed;
	}
};

class Velocity {
private:
	glm::vec3 v = glm::vec3(0, 0, 0.0f);
	float vr = 0;
public:
	Velocity() {};
	void accel(float x, float y, float z);
	void accelR(float r) { vr += r; };
	void reset();
	glm::vec3 getV() const { return v; };
	float getVr() const { return vr; }
};

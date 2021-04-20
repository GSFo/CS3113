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
	void move(glm::vec3,float);
};




class Status {
private:
	float speed;
public:
	//Status() {};
	Status(float speed) {
		this->speed = speed;
	};
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

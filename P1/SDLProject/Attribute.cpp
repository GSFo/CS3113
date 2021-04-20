#include "Attribute.h"

Location::Location(float x, float y, float r) {
	this->location = glm::vec3(x, y, 0);
	this->r = r;
};
float Location::getX() const { return this->location.x; };
float Location::getY() const { return this->location.y; };
float Location::getR() const { return this->r; };

void Location::move(glm::vec3 v,float time) {
	location += time * v;
}

void Location::spin(const float r,float time) {
	this->r += time * r;
}


void Velocity::reset() {
	v = glm::vec3(0.0f, 0.0f, 0.0f);
}

void Velocity::accel(float x, float y, float z) {
	v += glm::vec3(x, y, z);
}
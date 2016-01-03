#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(float x, float y, float z) {
	setDirection(x, y, z);
}

DirectionalLight::~DirectionalLight() {
}

void DirectionalLight::setDirection(float x, float y, float z) { 
	direction.x = x; 
	direction.y = y; 
	direction.z = z; 

	direction = glm::normalize(direction);
}
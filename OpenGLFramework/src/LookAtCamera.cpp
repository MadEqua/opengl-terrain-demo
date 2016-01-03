#include "LookAtCamera.h"

LookAtCamera::LookAtCamera() {
}

LookAtCamera::~LookAtCamera() {
}

void LookAtCamera::setPositionAndLookAt(const glm::vec3 &position, const glm::vec3 &lookAt, const glm::vec3 &upVector) {
	up = upVector;
	transform.setPosition(position, WORLD);
	this->lookAt.x = lookAt.x;
	this->lookAt.y = lookAt.y;
	this->lookAt.z = lookAt.z;
	updateInternals();
}

void LookAtCamera::setPosition(float x, float y, float z) { 
	transform.setPosition(x, y, z, WORLD);
	updateInternals();
}

void LookAtCamera::setLookAt(float x, float y, float z) { 
	lookAt.x = x;
	lookAt.y = y;
	lookAt.z = z;

	updateInternals();
}

void LookAtCamera::updateInternals() {
	glm::mat4 lookat = glm::lookAt(transform.getPosition(), lookAt, up);
	transform.setLocalXVector(lookat[0].x, lookat[1].x, lookat[2].x);
	transform.setLocalYVector(lookat[0].y, lookat[1].y, lookat[2].y);
	transform.setLocalZVector(lookat[0].z, lookat[1].z, lookat[2].z);

	frustum.setCameraPosition(transform.getPosition());
	frustum.setCameraOrientation(-transform.getLocalZVector(), transform.getLocalYVector(), transform.getLocalXVector());

	dirtyFlag = true;
}
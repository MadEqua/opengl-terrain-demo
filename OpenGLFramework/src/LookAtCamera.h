#pragma once

#include "Camera.h"
#include <GLFW\glfw3.h>

class LookAtCamera : public Camera
{
public:
	LookAtCamera();
	~LookAtCamera();

	void setPositionAndLookAt(const glm::vec3 &position, const glm::vec3 &lookAt, const glm::vec3 &upVector);

	void setPosition(float x, float y, float z);
	void setLookAt(float x, float y, float z);

private:
	glm::vec3 lookAt;
	glm::vec3 up;

	void updateInternals();
};


#pragma once

#include "Light.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight(float x, float y, float z);
	~DirectionalLight();

	inline glm::vec3& getDirection() { return direction; }
	void setDirection(float x, float y, float z);

protected:
	glm::vec3 direction;
};


#pragma once

#include <glm/glm.hpp>

class Light
{
public:
	Light();
	~Light();

	inline void setColor(float r, float g, float b) { color.r = r; color.g = g; color.b = b; }
	inline const glm::vec3& getColor() { return color; }

protected:
	glm::vec3 color;
};


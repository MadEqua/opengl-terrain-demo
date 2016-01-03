#pragma once

#include <glm/glm.hpp>

class AABB
{
public:
	AABB();
	AABB(const glm::vec3 &center, const glm::vec3 &dimensions);
	//AABB(const glm::vec3 &min, const glm::vec3 &max);
	~AABB();

	void setCenterAndDimensions(const glm::vec3 &center, const glm::vec3 &dimensions);

	inline const glm::vec3& getCenter() const { return center; }
	inline const glm::vec3& getDimensions() const { return dimensions; }

	inline const glm::vec3& getMin() const { return min; }
	inline const glm::vec3& getMax() const { return max; }

private:
	glm::vec3 center;
	glm::vec3 dimensions;

	glm::vec3 min;
	glm::vec3 max;
};


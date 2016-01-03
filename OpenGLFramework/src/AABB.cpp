#include "AABB.h"

AABB::AABB() : min(0), max(0){
}

AABB::AABB(const glm::vec3 &center, const glm::vec3 &dimensions) {
	setCenterAndDimensions(center, dimensions);
}

/*::AABB(const glm::vec3 &min, const glm::vec3 &max) : min(min), max(max) {
}*/

AABB::~AABB() {
}

void AABB::setCenterAndDimensions(const glm::vec3 &center, const glm::vec3 &dimensions) {
	min = center - dimensions / 2.0f;
	max = center + dimensions / 2.0f;

	this->center = center;
	this->dimensions = dimensions;
}
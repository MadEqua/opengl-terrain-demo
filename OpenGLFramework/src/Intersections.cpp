#include "Intersections.h"

#include "Utils.h"


Intersections::Intersections() {
}

Intersections::~Intersections() {
}

//OUTSIDE -> Normal direction
IntersectionResult Intersections::planePointIntersection(const Plane &plane, const glm::vec3 &point) const {
	float v = glm::dot(plane.getNormal(), point) + plane.getD();
	
	if (Utils::floatEquals(v, 0.0f))
		return INTERSECTING;
	else if (v < 0.0f)
		return INSIDE;
	else
		return OUTSIDE;
}

//OUTSIDE -> Normal direction
IntersectionResult Intersections::planeAABBIntersection(const Plane &plane, const AABB &aabb) const {
	const glm::vec3 center = aabb.getCenter();
	const glm::vec3 halfDiagonal = (aabb.getMax() - aabb.getMin()) * 0.5f;
	const glm::vec3 normal = plane.getNormal();

	float e = halfDiagonal.x * glm::abs(normal.x) + halfDiagonal.y * glm::abs(normal.y) + halfDiagonal.z * glm::abs(normal.z);
	float s = glm::dot(center, normal) + plane.getD();
	
	if (s - e > 0.0f)
		return OUTSIDE;
	else if (s + e < 0.0f)
		return INSIDE;
	else
		return INTERSECTING;
}

IntersectionResult Intersections::AABBAABBIntersection(const AABB &aabb1, const AABB &aabb2) const {
	const glm::vec3 min1 = aabb1.getMin();
	const glm::vec3 max1 = aabb1.getMax();

	const glm::vec3 min2 = aabb2.getMin();
	const glm::vec3 max2 = aabb2.getMax();

	if (min1.x > min2.x && max2.x > max1.x &&
		min1.y > min2.y && max2.y > max1.y &&
		min1.z > min2.z && max2.z > max1.z)
		return INSIDE;
	else if (min1.x > max2.x || min2.x > max1.x ||
		min1.y > max2.y || min2.y > max1.y ||
		min1.z > max2.z || min2.z > max1.z)
		return OUTSIDE;
	else
		return INTERSECTING;
}

IntersectionResult Intersections::AABBAPointIntersection(const AABB &aabb, const glm::vec3 point) const {
	const glm::vec3 min = aabb.getMin();
	const glm::vec3 max = aabb.getMax();

	if (point.x > min.x && max.x > point.x &&
		point.y > min.y && max.y > point.y &&
		point.z > min.z && max.z > point.z)
		return INSIDE;
	else if (point.x > max.x || min.x > point.x ||
		point.y > max.y || min.y > point.y ||
		point.z > max.z || min.z > point.z)
		return OUTSIDE;
	else
		return INTERSECTING;
}

IntersectionResult Intersections::frustumPointIntersection(Frustum &frustum, const glm::vec3 point) const {
	const Plane* planes = frustum.getPlanes();
	bool intersecting = false;

	for (int i = 0; i < 6; ++i) {
		IntersectionResult result = planePointIntersection(planes[i], point);
		if (result == OUTSIDE)
			return OUTSIDE;
		else if (result == INTERSECTING)
			intersecting = true;
	}

	if (intersecting)
		return INTERSECTING;
	else
		return INSIDE;
}

IntersectionResult Intersections::frustumAABBIntersection(Frustum &frustum, const AABB &aabb) const {
	const Plane* planes = frustum.getPlanes();
	bool intersecting = false;

	for (int i = 0; i < 6; ++i) {
		IntersectionResult result = planeAABBIntersection(planes[i], aabb);
		if (result == OUTSIDE)
			return OUTSIDE;
		else if (result == INTERSECTING)
			intersecting = true;
	}

	if (intersecting)
		return INTERSECTING;
	else
		return INSIDE;
}
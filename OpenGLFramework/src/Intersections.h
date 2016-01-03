#pragma once

#include "Plane.h"
#include "AABB.h"
#include "Frustum.h"

enum IntersectionResult {
	OUTSIDE,
	INSIDE,
	INTERSECTING,
};

class Intersections
{
public:
	static Intersections& getInstance() {
		// Guaranteed to be lazy initialized
		// Guaranteed that it will be destroyed correctly
		static Intersections instance;
		return instance;
	}

	IntersectionResult planePointIntersection(const Plane &plane, const glm::vec3 &point) const;
	IntersectionResult planeAABBIntersection(const Plane &plane, const AABB &aabb) const;
	
	IntersectionResult AABBAABBIntersection(const AABB &aabb1, const AABB &aabb2) const;
	IntersectionResult AABBAPointIntersection(const AABB &aabb, const glm::vec3 point) const;

	IntersectionResult frustumPointIntersection(Frustum &frustum, const glm::vec3 point) const;
	IntersectionResult frustumAABBIntersection(Frustum &frustum, const AABB &aabb) const;

private:
	Intersections();
	~Intersections();

	Intersections(Intersections const& copy);            // Not Implemented
	Intersections& operator=(Intersections const& copy); // Not Implemented
};


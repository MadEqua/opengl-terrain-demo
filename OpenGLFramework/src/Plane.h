#pragma once

#include <glm/glm.hpp>

/*
* Defines a 3D plane
* A point r = (x, y, z) is on the plane if the vector r - P is perpendicular to N
* N . (r - P) = 0
* N = (A, B, C) = normal of the plane
* P = (px, py, pz) = point on the plane
*
* A(x - px) + B(y - py) + C(z - pz) = 0
* Ax + By + Cz + D = 0
* with D = -Apx - Bpy - Cpz = -N.P
*/
class Plane
{
public:
	Plane();
	Plane(const glm::vec3 &normal, const glm::vec3 &point);
	Plane(const glm::vec3 &point1, const glm::vec3 &point2, const glm::vec3 &point3);
	~Plane();

	void modify(const glm::vec3 &normal, const glm::vec3 &point);
	void modify(const glm::vec3 &point1, const glm::vec3 &point2, const glm::vec3 &point3);

	float distanceToPointAndSide(const glm::vec3 &point) const; //can return negatives meaning point on the oposite side of N

	inline const glm::vec3& getNormal() const { return N; }
	inline const glm::vec3& getPoint() const { return P; }
	inline float getD() const { return D; }

private:
	glm::vec3 N;
	glm::vec3 P;
	float D;
};
#include "Plane.h"
#include "Utils.h"

Plane::Plane() : N(0, 0, 1), P(0), D(0) {
}

Plane::Plane(const glm::vec3 &normal, const glm::vec3 &point) {
	modify(normal, point);
}

Plane::Plane(const glm::vec3 &point1, const glm::vec3 &point2, const glm::vec3 &point3) {
	modify(point1, point2, point3);
}

Plane::~Plane() {
}

void Plane::modify(const glm::vec3 &normal, const glm::vec3 &point) {
	N = glm::normalize(normal);
	P = point;
	D = glm::dot(-N, point);
}

//Expecting CCW winding to compute the correct normal ('outwards' the plane)
void Plane::modify(const glm::vec3 &point1, const glm::vec3 &point2, const glm::vec3 &point3) {
	glm::vec3 v = point2 - point1;
	glm::vec3 u = point3 - point1;

	N = glm::cross(u, v);
	N = glm::normalize(N);

	P = point1;

	D = glm::dot(-N, point1);
}

float Plane::distanceToPointAndSide(const glm::vec3 &point) const {
	return glm::dot(N, point) + D;
}
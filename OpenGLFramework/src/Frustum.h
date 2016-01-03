#pragma once

#include "Plane.h"

#include <glm/glm.hpp>

enum PlaneName {
	NEAR = 0,
	FAR,
	LEFT,
	RIGHT,
	TOP,
	BOTTOM
};

const int PLANE_COUNT = 6;

class Frustum
{
public:
	Frustum();
	~Frustum();

	void setProjectionData(const float near, const float far, const float fovy, const float aspectRatio);
	void setCameraData(const glm::vec3 &position, const glm::vec3 &view, const glm::vec3 &up);

	void setCameraPosition(const glm::vec3 &position);
	void setCameraOrientation(const glm::vec3 &view, const glm::vec3 &up, const glm::vec3 &right);

	inline const Plane* getPlanes() { if (planesDirtyFlag) updatePlanes();  return planes; }
	const float* getPackedPlaneData(); //useful to send to shaders

	float fovy; //degrees
	float aspectRatio;
	float near, far;
	glm::vec2 nearDimensions;
	glm::vec2 farDimensions;

private:
	//in world coordinates
	glm::vec3 position;
	glm::vec3 view;
	glm::vec3 up;
	glm::vec3 right;

	float packedPlaneData[24]; //6 times N.x, N.y, N.z and D

	//near, far, left, right, up, down
	Plane planes[PLANE_COUNT];
	bool planesDirtyFlag;
	void updatePlanes();
};
#include "Frustum.h"

Frustum::Frustum() : planesDirtyFlag(true) {
}

Frustum::~Frustum() {
}

void Frustum::setProjectionData(const float near, const float far, const float fovy, const float aspectRatio) {
	this->near = near;
	this->far = far;
	this->fovy = fovy;
	this->aspectRatio = aspectRatio;

	float tg = glm::tan(glm::radians(fovy * 0.5f));
	nearDimensions.y = tg * near * 2.0f;
	nearDimensions.x = nearDimensions.y * aspectRatio;

	farDimensions.y = tg * far * 2.0f;
	farDimensions.x = farDimensions.y * aspectRatio;

	planesDirtyFlag = true;
}
void Frustum::setCameraData(const glm::vec3 &position, const glm::vec3 &view, const glm::vec3 &up) {
	setCameraPosition(position);
	setCameraOrientation(view, up, glm::cross(view, up));
}

void Frustum::setCameraPosition(const glm::vec3 &position) {
	this->position = position;
	planesDirtyFlag = true;
}

void Frustum::setCameraOrientation(const glm::vec3 &view, const glm::vec3 &up, const glm::vec3 &right) {
	this->view = view;
	this->up = up;
	this->right = right;
	planesDirtyFlag = true;
}

void Frustum::updatePlanes() {
	glm::vec3 nearc = position + (view * near);
	planes[NEAR].modify(-view, nearc);

	glm::vec3 farc = position + (view * far);
	planes[FAR].modify(view, farc);

	glm::vec3 a = (nearc + right * nearDimensions.x / 2.0f) - position;
	a = glm::normalize(a);
	glm::vec3 normal = glm::cross(a, up);
	planes[RIGHT].modify(normal, position);

	a = (nearc - right * nearDimensions.x / 2.0f) - position;
	a = glm::normalize(a);
	normal = glm::cross(up, a);
	planes[LEFT].modify(normal, position);

	a = (nearc + up * nearDimensions.y / 2.0f) - position;
	a = glm::normalize(a);
	normal = glm::cross(right, a);
	planes[TOP].modify(normal, position);

	a = (nearc - up * nearDimensions.y / 2.0f) - position;
	a = glm::normalize(a);
	normal = glm::cross(a, right);
	planes[BOTTOM].modify(normal, position);

	planesDirtyFlag = false;
}

const float* Frustum::getPackedPlaneData() {
	if (planesDirtyFlag) 
		updatePlanes();
	
	for (int i = 0; i < 6; ++i) {
		Plane &plane = planes[i];
		float *ptr = packedPlaneData + (i * 4);
		memcpy(ptr, &plane.getNormal(), 3 * sizeof(float));
		*(ptr + 3) = plane.getD();
	}
	return packedPlaneData;
}
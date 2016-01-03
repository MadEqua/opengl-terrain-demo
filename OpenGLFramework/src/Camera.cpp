#include "Camera.h"

Camera::Camera() : dirtyFlag(true) {
	frustum.setProjectionData(0.1f, 100.0f, 45.0f, 1.0f);
	frustum.setCameraData(transform.getPosition(), -transform.getLocalZVector(), transform.getLocalYVector());
}

Camera::~Camera() {
}

const glm::mat4& Camera::getViewMatrix() {
	return transform.getInverseTransformationMatrix();
}

const glm::mat4& Camera::getProjectionMatrix() {
	if (projectionMatrixDirtyFlag) {
		projectionMatrix = glm::perspective(glm::radians(frustum.fovy), frustum.aspectRatio, frustum.near, frustum.far);
		projectionMatrixDirtyFlag = false;
	}
	return projectionMatrix;
}

void Camera::setProjectionMatrix(float fovy, float aspectRatio, float near, float far) {
	frustum.setProjectionData(near, far, fovy, aspectRatio);
	projectionMatrixDirtyFlag = true;

	dirtyFlag = true;
}

void Camera::windowResizedCallback(const int width, const int height) {
	setProjectionMatrix(frustum.fovy, (float)width / (float)height, frustum.near, frustum.far);
	glViewport(0, 0, width, height);
}
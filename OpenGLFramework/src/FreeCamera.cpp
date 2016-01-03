#include "FreeCamera.h"

#include <GLFW\glfw3.h>

#include "Utils.h"

FreeCamera::FreeCamera(float velocity) : cursorLastPos(Utils::MAX_INT), inputVector(0),
	velocity(velocity), slowVelocity(velocity / 10.0f) {
}

FreeCamera::~FreeCamera() {
}

void FreeCamera::setPositionAndRotation(const glm::vec3 &position, const glm::vec3 &rotation) {
	transform.setPosition(position.x, position.y, position.z, WORLD);
	transform.setRotation(rotation.x, rotation.y, rotation.z, WORLD, LOCAL);

	const glm::mat4 &matrix = transform.getTransformationMatrix();
	frustum.setCameraData(position, -transform.getLocalZVector(), transform.getLocalYVector());

	dirtyFlag = true;
}

void FreeCamera::moveForwards(float ammount, Space space) {
	transform.translateBy(0.0f, 0.0f, -ammount, space);
	frustum.setCameraPosition(transform.getPosition());
	dirtyFlag = true;
}

void FreeCamera::moveBackwards(float ammount, Space space) {
	transform.translateBy(0.0f, 0.0f, ammount, space);
	frustum.setCameraPosition(transform.getPosition());
	dirtyFlag = true;
}

void FreeCamera::strafeLeft(float ammount, Space space) {
	transform.translateBy(-ammount, 0.0f, 0.0f, space);
	frustum.setCameraPosition(transform.getPosition());
	dirtyFlag = true;
}

void FreeCamera::strafeRight(float ammount, Space space) {
	transform.translateBy(ammount, 0.0f, 0.0f, space);
	frustum.setCameraPosition(transform.getPosition());
	dirtyFlag = true;
}

void FreeCamera::pitch(float ammount, Space space) {
	transform.rotateBy(ammount, 0.0f, 0.0f, space, LOCAL);
	frustum.setCameraOrientation(-transform.getLocalZVector(), transform.getLocalYVector(), transform.getLocalXVector());
	dirtyFlag = true;
}

void FreeCamera::roll(float ammount, Space space) {
	transform.rotateBy(0.0f, 0.0f, ammount, space, LOCAL);
	frustum.setCameraOrientation(-transform.getLocalZVector(), transform.getLocalYVector(), transform.getLocalXVector());
}

void FreeCamera::yaw(float ammount, Space space) {
	transform.rotateBy(0.0f, ammount, 0.0f, space, LOCAL);
	frustum.setCameraOrientation(-transform.getLocalZVector(), transform.getLocalYVector(), transform.getLocalXVector());
	dirtyFlag = true;
}

void FreeCamera::windowResizedCallback(const int width, const int height) {
	Camera::windowResizedCallback(width, height);
	cursorLastPos.x = cursorLastPos.y = Utils::MAX_INT;
}

void FreeCamera::keyCallback(const int key, const int scanCode, const int action, const int mods) {
	if (action == GLFW_RELEASE) {
		if (key == 'W' || key == 'S')
			inputVector.x = 0;
		if (key == 'A' || key == 'D')
			inputVector.y = 0;
	}
	else if (action == GLFW_PRESS) {
		if (key == 'W')
			inputVector.x = -1;
		else if (key == 'S')
			inputVector.x = 1;

		if (key == 'A')
			inputVector.y = -1;
		else if (key == 'D')
			inputVector.y = 1;
	}

	if (mods == GLFW_MOD_SHIFT)
		inputVector.z = 1;
	else
		inputVector.z = 0;
}

void FreeCamera::cursorPositionCallback(const double xpos, const double ypos) {
	int dx, dy;

	if (cursorLastPos.x != Utils::MAX_INT) {
		dx = int(cursorLastPos.x - float(xpos));
		yaw(dx * 0.1f, WORLD);
	}

	if (cursorLastPos.y != Utils::MAX_INT) {
		dy = int(cursorLastPos.y - float(ypos));
		pitch(dy * 0.1f, LOCAL);
	}

	cursorLastPos.x = static_cast<int>(xpos);
	cursorLastPos.y = static_cast<int>(ypos);
}

void FreeCamera::update(double deltaTime) {
	float vel = inputVector.z ? slowVelocity : velocity;

	if (inputVector.x < 0)
		moveForwards((float)-inputVector.x * vel * (float)deltaTime);
	else if (inputVector.x > 0)
		moveBackwards((float)inputVector.x * vel * (float)deltaTime);

	if (inputVector.y < 0)
		strafeLeft((float)-inputVector.y * vel * (float)deltaTime);
	else if (inputVector.y > 0)
		strafeRight((float)inputVector.y * vel * (float)deltaTime);
}
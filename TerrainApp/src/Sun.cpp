#include "Sun.h"
#include <GLFW\glfw3.h>

#include <glm/gtc/type_ptr.hpp>

#include "Application.h"

Sun::Sun(float x, float y, float z) : directionalLight(x, y, z) {
}

Sun::~Sun() {
}

void Sun::initialize() {
	transform.setPosition(DISTANCE, 0.0f, 0.0f, WORLD);
	transform.setRotation(0.0f, 90.0f, 0.0f, LOCAL, LOCAL);
	transform.rotateBy(0.0f, 0.0f, 25.0f, WORLD, WORLD);
	computeDirection();
}

void Sun::render(int passID) {
	if (passID == 0) {
		material->bind();

		glDisable(GL_DEPTH_TEST);

		Camera &camera = application->getActiveCamera();
		glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(camera.getProjectionMatrix() *
			camera.getViewMatrix() *
			transform.getTransformationMatrix()));

		mesh->renderIndexed();
		glEnable(GL_DEPTH_TEST);
	}
}

void Sun::update(double deltaTime) {
}

void Sun::keyCallback(const int key, const int scanCode, const int action, const int mods) {
	const float VEL = 1.0f;
	
	if (key == GLFW_KEY_LEFT) {
		transform.rotateBy(0.0f, 0.0f, VEL, WORLD, WORLD);
		computeDirection();
	}
	else if (key == GLFW_KEY_RIGHT) {
		transform.rotateBy(0.0f, 0.0f, -VEL, WORLD, WORLD);
		computeDirection();
	}
}

void Sun::computeDirection() {
	const glm::vec3 &dir = -transform.getPosition();
	directionalLight.setDirection(dir.x, dir.y, dir.z);
}
#pragma once

#include "Camera.h"

class FreeCamera : public Camera
{
public:
	FreeCamera(float velocity = 400.0f);
	~FreeCamera();

	virtual void windowResizedCallback(const int width, const int height);
	virtual void keyCallback(const int key, const int scanCode, const int action, const int mods);
	virtual void cursorPositionCallback(const double xpos, const double ypos);

	virtual void update(double deltaTime);

	void setPositionAndRotation(const glm::vec3 &position, const glm::vec3 &rotation);

	//Camera movement interface
	void moveForwards(float ammount, Space space = LOCAL);
	void moveBackwards(float ammount, Space space = LOCAL);
	void strafeLeft(float ammount, Space space = LOCAL);
	void strafeRight(float ammount, Space space = LOCAL);

	void pitch(float ammount, Space space = LOCAL); //x
	void roll(float ammount, Space space = LOCAL); //z
	void yaw(float ammount, Space space = LOCAL); //y


private:
	glm::ivec2 cursorLastPos;
	glm::ivec3 inputVector; //w/s, a/d, shift?

	float velocity, slowVelocity;
};


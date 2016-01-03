#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "SceneObject.h"
#include "Frustum.h"

/*
* Encapsulating view and projection matrices.
* Also maintains a Frustum to perform culling against.
*/
class Camera : public SceneObject
{
public:
	Camera();
	~Camera();

	const glm::mat4& getViewMatrix();
	const glm::mat4& getProjectionMatrix();
	inline Frustum& getFrustum() { return frustum; }

	//fovy in degrees
	void setProjectionMatrix(float fovy, float aspectRatio, float near, float far);

	//TODO:  this will only work for 1 call place
	inline bool changedSinceLastCall() { bool temp = dirtyFlag; dirtyFlag = false; return temp; }

	virtual void windowResizedCallback(const int width, const int height);

protected:
	glm::mat4 projectionMatrix;
	bool projectionMatrixDirtyFlag;

	Frustum frustum;

	bool dirtyFlag;
};


#include "Transform.h"

Transform::Transform() : position(0.0f), scale(1.0f), localAxis(1.0f) {
	transformationMatrixDirtyFlag = true;
	normalMatrixDirtyFlag = true;
	inverseTransformationMatrixDirtyFlag = true;
}

Transform::~Transform() {
}

const glm::mat4& Transform::getTransformationMatrix() {
	if (transformationMatrixDirtyFlag) {
		transformationMatrix = glm::mat4(1.0f);
		transformationMatrix = glm::translate(transformationMatrix, position);
		transformationMatrix *= localAxis;
		transformationMatrix = glm::scale(transformationMatrix, scale);

		transformationMatrixDirtyFlag = false;
	}

	return transformationMatrix;
}

const glm::mat4& Transform::getInverseTransformationMatrix() {
	if (inverseTransformationMatrixDirtyFlag) {
		inverseTransformationMatrix = glm::mat4(1.0f);
		inverseTransformationMatrix = glm::scale(inverseTransformationMatrix, 1.0f / scale);
		inverseTransformationMatrix *= glm::transpose(localAxis);
		inverseTransformationMatrix = glm::translate(inverseTransformationMatrix, -position);
		inverseTransformationMatrixDirtyFlag = false;
	}

	return inverseTransformationMatrix;
}

const glm::mat3& Transform::getNormalMatrix() {
	if (normalMatrixDirtyFlag) {
		//uniform scaling -> normal matrix = transformation matrix
		if (scale.x == scale.y && scale.x == scale.z) {
			normalMatrix[0][0] = localAxis[0][0];
			normalMatrix[0][1] = localAxis[0][1];
			normalMatrix[0][2] = localAxis[0][2];

			normalMatrix[1][0] = localAxis[1][0];
			normalMatrix[1][1] = localAxis[1][1];
			normalMatrix[1][2] = localAxis[1][2];

			normalMatrix[2][0] = localAxis[2][0];
			normalMatrix[2][1] = localAxis[2][1];
			normalMatrix[2][2] = localAxis[2][2];
		}
		//else compute transposed inverse of the transformation matrix
		else {
			const glm::mat4 &inverse = getInverseTransformationMatrix();

			normalMatrix[0][0] = inverse[0][0];
			normalMatrix[0][1] = inverse[1][0];
			normalMatrix[0][2] = inverse[2][0];

			normalMatrix[1][0] = inverse[0][1];
			normalMatrix[1][1] = inverse[1][1];
			normalMatrix[1][2] = inverse[2][1];

			normalMatrix[2][0] = inverse[0][2];
			normalMatrix[2][1] = inverse[1][2];
			normalMatrix[2][2] = inverse[2][2];
		}
		normalMatrixDirtyFlag = false;
	}

	return normalMatrix;
}

void Transform::setPosition(float x, float y, float z, Space space) {
	if (space == WORLD) {
		position.x = x;
		position.y = y;
		position.z = z;
	}
	else if (space == LOCAL) {
		glm::vec4 pos = getTransformationMatrix() * glm::vec4(x, y, z, 1.0f);
		position.x = pos.x;
		position.y = pos.y;
		position.z = pos.z;
	}
	transformationMatrixDirtyFlag = true;
	inverseTransformationMatrixDirtyFlag = true;
}

void Transform::setPosition(const glm::vec3 &pos, Space space) {
	setPosition(pos.x, pos.y, pos.z, space);
}

void Transform::setRotation(float x, float y, float z, Space space, Space pivot) {
	glm::mat4 r = glm::mat4(1.0f);
	r = glm::rotate(r, glm::radians(z), glm::vec3(0, 0, 1));
	r = glm::rotate(r, glm::radians(x), glm::vec3(1, 0, 0));
	r = glm::rotate(r, glm::radians(y), glm::vec3(0, 1, 0));

	if (space == WORLD) {
		localAxis = r;
	}
	else if (space == LOCAL) {
		localAxis = localAxis * r;
	}

	//Rotation around world origin will change the position
	if (pivot == WORLD) {
		glm::mat4 m = glm::mat4(1.0f);
		m[3].x = position.x;
		m[3].y = position.y;
		m[3].z = position.z;
		m = r * m;
		position.x = m[3].x;
		position.y = m[3].y;
		position.z = m[3].z;
	}

	transformationMatrixDirtyFlag = true;
	inverseTransformationMatrixDirtyFlag = true;
	normalMatrixDirtyFlag = true;
}

void Transform::setScale(float x, float y, float z, Space space) {
	/*if (space == WORLD) {
		//TODO
	}
	else if (space == LOCAL) {
		scale.x = x;
		scale.y = y;
		scale.z = z;
	}*/

	scale.x = x;
	scale.y = y;
	scale.z = z;


	transformationMatrixDirtyFlag = true;
	inverseTransformationMatrixDirtyFlag = true;
	normalMatrixDirtyFlag = true;
}

void Transform::translateBy(float x, float y, float z, Space space) {
	if (space == WORLD) {
		position.x += x;
		position.y += y;
		position.z += z;
	}
	else if (space == LOCAL) {
		glm::vec4 p = getTransformationMatrix() * glm::vec4(x, y, z, 1.0f);
		position.x = p.x;
		position.y = p.y;
		position.z = p.z;
	}
	transformationMatrixDirtyFlag = true;
	inverseTransformationMatrixDirtyFlag = true;
}

void Transform::rotateBy(float x, float y, float z, Space space, Space pivot) {
	
	glm::mat4 r = glm::mat4(1.0f);
	r = glm::rotate(r, glm::radians(z), glm::vec3(0, 0, 1));
	r = glm::rotate(r, glm::radians(x), glm::vec3(1, 0, 0));
	r = glm::rotate(r, glm::radians(y), glm::vec3(0, 1, 0));
	
	
	if (space == WORLD) {
		localAxis = r * localAxis;
	}
	else if (space == LOCAL) {
		localAxis = localAxis * r;
	}

	//Rotation around world origin will change the position
	if (pivot == WORLD) {
		glm::mat4 m = glm::mat4(1.0f);
		m[3].x = position.x;
		m[3].y = position.y;
		m[3].z = position.z;
		m = r * m;
		position.x = m[3].x;
		position.y = m[3].y;
		position.z = m[3].z;
	}

	transformationMatrixDirtyFlag = true;
	inverseTransformationMatrixDirtyFlag = true;
	normalMatrixDirtyFlag = true;
}

void Transform::scaleBy(float x, float y, float z, Space space) {
	if (space == WORLD) {
		//TODO
	}
	else if (space == LOCAL) {
		scale.x += x;
		scale.y += y;
		scale.z += z;
	}
	transformationMatrixDirtyFlag = true;
	inverseTransformationMatrixDirtyFlag = true;
	normalMatrixDirtyFlag = true;
}
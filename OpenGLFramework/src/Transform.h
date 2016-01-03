#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Space {
	WORLD, LOCAL
};

class Transform
{
public:
	Transform();
	~Transform();

	void setPosition(float x, float y, float z, Space space);
	void setPosition(const glm::vec3 &pos, Space space);

	void setRotation(float x, float y, float z, Space space, Space pivot); //space -> space of the angle values, pivot -> rotation pivot point (WORLD or LOCAL origins)
	void setScale(float x, float y, float z, Space space);

	void translateBy(float x, float y, float z, Space space);
	void rotateBy(float x, float y, float z, Space space, Space pivot);
	void scaleBy(float x, float y, float z, Space space);

	inline const glm::vec3& getPosition() const { return position; }
	//inline glm::vec3& getRotation() { return rotation; } //TODO
	inline const glm::vec3& getScale() const { return scale; }

	inline void setLocalXVector(float x, float y, float z) { localAxis[0].x = x; localAxis[0].y = y; localAxis[0].z = z; transformationMatrixDirtyFlag = true; inverseTransformationMatrixDirtyFlag = true; normalMatrixDirtyFlag = true; }
	inline void setLocalYVector(float x, float y, float z) { localAxis[1].x = x; localAxis[1].y = y; localAxis[1].z = z; transformationMatrixDirtyFlag = true; inverseTransformationMatrixDirtyFlag = true; normalMatrixDirtyFlag = true; }
	inline void setLocalZVector(float x, float y, float z) { localAxis[2].x = x; localAxis[2].y = y; localAxis[2].z = z; transformationMatrixDirtyFlag = true; inverseTransformationMatrixDirtyFlag = true; normalMatrixDirtyFlag = true; }

	inline const glm::vec3& getLocalXVector() { tempVector.x = localAxis[0].x; tempVector.y = localAxis[0].y; tempVector.z = localAxis[0].z; return tempVector; }
	inline const glm::vec3& getLocalYVector() { tempVector2.x = localAxis[1].x; tempVector2.y = localAxis[1].y; tempVector2.z = localAxis[1].z; return tempVector2; }
	inline const glm::vec3& getLocalZVector() { tempVector3.x = localAxis[2].x; tempVector3.y = localAxis[2].y; tempVector3.z = localAxis[2].z; return tempVector3; }

	const glm::mat4& getTransformationMatrix();
	const glm::mat4& getInverseTransformationMatrix();
	const glm::mat3& getNormalMatrix();

private:
	//In world coordinates
	glm::vec3 position;
	glm::vec3 scale;
	glm::mat4 localAxis; //representing rotation only, mat4 for convenience

	glm::mat4 transformationMatrix; //local to world
	glm::mat4 inverseTransformationMatrix; //world to local
	glm::mat3 normalMatrix;

	//for returns
	glm::vec3 tempVector;
	glm::vec3 tempVector2;
	glm::vec3 tempVector3;

	bool inverseTransformationMatrixDirtyFlag;
	bool transformationMatrixDirtyFlag;
	bool normalMatrixDirtyFlag;
};


#pragma once

#include "SceneObject.h"
#include "Camera.h"
#include "Sun.h"

class Terrain : public SceneObject
{
public:
	Terrain(Sun *sun);
	virtual ~Terrain();

	virtual void initialize();
	virtual void render(int passID);
	virtual void update(double deltaTime);

	virtual void keyCallback(const int key, const int scanCode, const int action, const int mods);

private:
	float displacementScale;
	float tessellatedTriWidth;
	glm::ivec2 gridsize;
	glm::vec2 size;
	int patchCount;

	Camera *camera;
	Sun *sun;

	glm::mat4 topViewMatrix;

	bool drawWireframe;
	bool enableFog;	
	bool heightBasedTexturing;

	//depth pass stuff
	glm::mat4 sunProjectionMatrix;
	glm::mat4 bias;
};
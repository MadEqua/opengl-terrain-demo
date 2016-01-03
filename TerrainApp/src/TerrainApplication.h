#pragma once

#include "Application.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "FBO.h"

#include "Automation.h"

class TerrainApplication : public Application {
public:
	TerrainApplication();
	~TerrainApplication();

	virtual void initialize();
	virtual void render();
	virtual void update(double deltaTime);

	virtual void windowResizedCallback(const int width, const int height);
	virtual void keyCallback(const int key, const int scanCode, const int action, const int mods);
	virtual void cursorPositionCallback(const double xpos, const double ypos);
	virtual void mouseButtonCallback(const int button, const int action, const int mods);

	bool enableShadowMapping;

private:
	int terrainID, skyID, sunID;
	FBO *depthFBO;

	Automation<glm::vec3> *cameraPositionAutomation;
	Automation<glm::vec3> *cameraLookAtAutomation;

	void initFBO();
};

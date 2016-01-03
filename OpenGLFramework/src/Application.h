#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <vector>

#include "SceneObject.h"
#include "EventListener.h"
#include "Camera.h"
#include "SkyBox.h"

class Application : public EventListener {
public:
	Application();
	~Application();

	virtual void initialize();
	virtual void render();
	virtual void update(double deltaTime);

	virtual void windowResizedCallback(const int width, const int height);
	virtual void keyCallback(const int key, const int scanCode, const int action, const int mods);
	virtual void cursorPositionCallback(const double xpos, const double ypos);
	virtual void mouseButtonCallback(const int button, const int action, const int mods);
	
	bool initGLContext(int samples = 0, bool srgbFrameBuffer = true);
	void startLoop();

	void addCamera(Camera *camera);
	int addSceneObject(SceneObject *sceneObject);
	int addEventListener(EventListener *eventListener);
	void setWindowProperties(const std::string &title, const int width, const int height);

	inline int getWindowWidth() const { return windowWidth; }
	inline int getWindowHeight() const { return windowHeight; }
	inline Camera& getCamera(int id) const { return *cameras[id]; }
	inline void setActiveCamera(int id) { activeCamera = id; }
	inline Camera& getActiveCamera() const { return *cameras[activeCamera]; }

protected:
	GLFWwindow* window;
	int windowWidth, windowHeight;
	std::string windowTitle;

	double lastFrameTime;
	double lastFPSShowTime;

	std::vector<SceneObject*> sceneObjects;
	std::vector<EventListener*> eventListeners;
	std::vector<Camera*> cameras; 
	int activeCamera;

	bool isCursorEnabled;
	bool vsyncEnabled;
	bool initialized;

	inline void setVsync() { glfwSwapInterval(vsyncEnabled ? 1 : 0); }
};
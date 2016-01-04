#include "Application.h"

#include <iostream>
#include <sstream>
#include "FreeCamera.h"

//GLOBAL APPLICATION REFERENCE
Application *app;

//GLOBAL CALLBACKS FOR GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	app->keyCallback(key, scancode, action, mods);
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
	app->cursorPositionCallback(xpos, ypos);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	app->mouseButtonCallback(button, action, mods);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	app->windowResizedCallback(width, height);
}

Application::Application() : windowWidth(200), windowHeight(200), windowTitle("OpenGL App"),
	isCursorEnabled(true), vsyncEnabled(true), 
	initialized(false), activeCamera(0) {
	app = this;
}

Application::~Application() {
	for (SceneObject *object : sceneObjects) {
		delete object;
	}
	for (Camera *c : cameras) {
		delete c;
	}
	cameras.clear();
	sceneObjects.clear();
	eventListeners.clear();
	glfwTerminate();
}

bool Application::initGLContext(int samples, bool srgbFrameBuffer) {
	if (!glfwInit())
		return false;

	glfwWindowHint(GLFW_SAMPLES, samples);
	glfwWindowHint(GLFW_SRGB_CAPABLE, srgbFrameBuffer ? GL_TRUE : GL_FALSE);

	window = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(), NULL, NULL);
	if (!window) {
		std::cout << "Error creating window.";
		return false;
	}

	glfwMakeContextCurrent(window);

	//GLEW
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cout << "Error initing Glew: " << glewGetErrorString(err);
		return false;
	}

	int res;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_BACK_LEFT, GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING, &res);

	if (res == GL_LINEAR) {
		std::cout << "Linear RGB Default Framebuffer.\n";
	}
	else if (res == GL_SRGB) {
		std::cout << "sRGB Default Framebuffer.\n";
		glEnable(GL_FRAMEBUFFER_SRGB); //enable auto Linear RGB to sRGB conversion when writing to sRGB framebuffers
	}

	glGetIntegerv(GL_SAMPLES, &res);
	if (res > 0) {
		std::cout << "Multisampled Default Framebuffer. Samples: " << res << std::endl;
		glEnable(GL_MULTISAMPLE);
	}
	else {
		std::cout << "Non-Multisampled Default Framebuffer.\n";
	}

	//Get version info
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	std::cout << "Renderer: " << renderer << std::endl;
	std::cout << "OpenGL version supported: " << version << std::endl;

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	setVsync();

	return true;
}

void Application::initialize() {
	for (SceneObject *object : sceneObjects) {
		object->initialize();
	}
	initialized = true;
}

void Application::startLoop() {
	double deltaTime, actualTime;
	lastFrameTime = glfwGetTime();
	lastFPSShowTime = 0;

	std::stringstream title;

	while (!glfwWindowShouldClose(window)) {
		actualTime = glfwGetTime();
		deltaTime = actualTime - lastFrameTime;
		lastFrameTime = actualTime;

		if (actualTime - lastFPSShowTime > 1.0) {
			title.str("");
			title << windowTitle << " [FPS: " << (1.0 / deltaTime) << "] [Frame Time: " << deltaTime << "]";
			glfwSetWindowTitle(window, title.str().c_str());
			//std::cout << (1.0 / deltaTime) << std::endl;
			lastFPSShowTime = actualTime;
		}

		glfwPollEvents();
		update(deltaTime);
		render();

		glfwSwapBuffers(window);
	}
}

void Application::render() {
	for (SceneObject *object : sceneObjects) {
		if (object->isRenderEnabled())
			object->render();
	}
}

void Application::update(double deltaTime) {
	cameras[activeCamera]->update(deltaTime);
	for (SceneObject *object : sceneObjects) {
		object->update(deltaTime);
	}
}

int Application::addSceneObject(SceneObject *sceneObject) {
	sceneObject->setApplication(this);
	sceneObjects.push_back(sceneObject);
	return sceneObjects.size() - 1;
}

int Application::addEventListener(EventListener *eventListener) {
	eventListeners.push_back(eventListener);
	return eventListeners.size() - 1;
}

void Application::addCamera(Camera *camera) {
	cameras.push_back(camera);
}

void Application::windowResizedCallback(const int width, const int height) {
	if (width > 0 && height > 0) {
		windowWidth = width;
		windowHeight = height;

		for (Camera *c : cameras) {
			c->windowResizedCallback(width, height);
		}
		for (EventListener *el : eventListeners) {
			el->windowResizedCallback(width, height);
		}
	}
}

void Application::keyCallback(const int key, const int scanCode, const int action, const int mods) {
	if (key == GLFW_KEY_ESCAPE)
		glfwSetWindowShouldClose(window, true);
	else if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_PRESS) {
		isCursorEnabled = !isCursorEnabled;
		int value = isCursorEnabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED;
		glfwSetInputMode(window, GLFW_CURSOR, value);
	}
	else if (key == GLFW_KEY_LEFT_ALT && action == GLFW_PRESS) {
		vsyncEnabled = !vsyncEnabled;
		setVsync();
	}

	cameras[activeCamera]->keyCallback(key, scanCode, action, mods);
	for (EventListener *el : eventListeners) {
		el->keyCallback(key, scanCode, action, mods);
	}
}

void Application::cursorPositionCallback(const double xpos, const double ypos) {
	cameras[activeCamera]->cursorPositionCallback(xpos, ypos);
	for (EventListener *el : eventListeners) {
		el->cursorPositionCallback(xpos, ypos);
	}
}	

void Application::mouseButtonCallback(const int button, const int action, const int mods) {
	cameras[activeCamera]->mouseButtonCallback(button, action, mods);
	for (EventListener *el : eventListeners) {
		el->mouseButtonCallback(button, action, mods);
	}
}

void Application::setWindowProperties(const std::string &title, const int width, const int height) {
	windowWidth = width;
	windowHeight = height;
	windowTitle = title;
	glfwSetWindowSize(window, width, height);
	glfwSetWindowTitle(window, title.c_str());

	glViewport(0, 0, windowWidth, windowHeight);
}
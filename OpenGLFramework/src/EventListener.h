#pragma once

class EventListener {
public:
	virtual void windowResizedCallback(const int width, const int height) {};
	virtual void keyCallback(const int key, const int scanCode, const int action, const int mods) {};
	virtual void cursorPositionCallback(const double xpos, const double ypos) {};
	virtual void mouseButtonCallback(const int button, const int action, const int mods) {};
};
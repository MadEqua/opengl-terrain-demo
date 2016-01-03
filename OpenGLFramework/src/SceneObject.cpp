#include "SceneObject.h"

#include <GL/glew.h>

SceneObject::SceneObject() : renderEnabled(true) {
}

SceneObject::~SceneObject() {
}

void SceneObject::initialize() {
}

void SceneObject::update(double deltaTime) {
}

void SceneObject::render(int passID) {
	bindMaterial();

	if (mesh != 0)
		mesh->renderIndexed();

}

void SceneObject::bindMaterial() {
	if (material != 0)
		material->bind();
}
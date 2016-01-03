#include "AABBDebugSceneObject.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Application.h"
#include "AssetManager.h"

#include "Material.h"
#include "Mesh.h"

AABBDebugSceneObject::AABBDebugSceneObject(const AABB &aabb) : aabb(aabb) {
}


AABBDebugSceneObject::~AABBDebugSceneObject() {
}

void AABBDebugSceneObject::initialize() {
	setMaterial(AssetManager::getInstance().getAsset<Material>("whiteMaterial"));
	setMesh(AssetManager::getInstance().getAsset<Mesh>("boxMesh"));

	glLineWidth(2);
}

void AABBDebugSceneObject::update(double deltaTime) {
	transform.setPosition(aabb.getCenter().x, aabb.getCenter().y, aabb.getCenter().z, WORLD);
	transform.setScale(aabb.getDimensions().x, aabb.getDimensions().y, aabb.getDimensions().z, WORLD);
}

void AABBDebugSceneObject::render(int passID) {
	bindMaterial();

	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(application->getActiveCamera().getProjectionMatrix() *
		application->getActiveCamera().getViewMatrix() *
		transform.getTransformationMatrix()));

	mesh->renderIndexed(GL_LINES);
}
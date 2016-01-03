#pragma once

#include "SceneObject.h"
#include "AABB.h"

class AABBDebugSceneObject : public SceneObject
{
public:
	AABBDebugSceneObject(const AABB &aabb);
	~AABBDebugSceneObject();

	virtual void initialize();
	virtual void update(double deltaTime);
	virtual void render(int passID);

private:
	const AABB &aabb;
};


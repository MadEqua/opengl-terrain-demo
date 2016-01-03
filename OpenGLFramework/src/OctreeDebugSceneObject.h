#pragma once

#include "SceneObject.h"
#include "Octree.h"

#include "AABBDebugSceneObject.h"

#include <vector>

template <class T>
class OctreeDebugSceneObject : public SceneObject
{
public:
	OctreeDebugSceneObject(Octree<T> *octree);
	~OctreeDebugSceneObject();

	virtual void initialize();
	virtual void render(int passID);

private:
	Octree<T> *octree;

	std::vector<AABBDebugSceneObject*> aabbDebugObjects;

	void build(Octree<T> *octree);
};
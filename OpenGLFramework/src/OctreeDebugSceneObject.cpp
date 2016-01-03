#include "OctreeDebugSceneObject.h"

template <class T>
OctreeDebugSceneObject<T>::OctreeDebugSceneObject(Octree<T> *octree) : octree(octree) {
}

template <class T>
OctreeDebugSceneObject<T>::~OctreeDebugSceneObject() {
}

template <class T>
void OctreeDebugSceneObject<T>::build(Octree<T> *octree) {
	AABBDebugSceneObject *object = new AABBDebugSceneObject(octree->boundingBox);
	object->setApplication(application);
	object->initialize();
	aabbDebugObjects.push_back(object);

	if (octree->hasChildren) {
		for (int i = 0; i < 8; ++i) {
			build(octree->childs[i]);
		}
	}
}

template <class T>
void OctreeDebugSceneObject<T>::initialize() {
	build(octree);
}

template <class T>
void OctreeDebugSceneObject<T>::render(int passID) {
	for (AABBDebugSceneObject *aabbso : aabbDebugObjects) {
		aabbso->render(deltaTime);
	}
}
#pragma once

#include "AABB.h"
#include "Frustum.h"
#include <list>


//TODO: finish it :)
template <class T>
class OctreeData {
public:
	OctreeData(const T data, const AABB &boundingBox) : data(data), boundingBox(boundingBox) {}

	T data;
	AABB boundingBox;
};

template <class T>
class Octree
{
	template <class T> friend class OctreeDebugSceneObject;

public:
	Octree(const AABB &rootBoundingBox, const std::list<OctreeData<T>> &data);
	Octree(const AABB &rootBoundingBox);
	Octree();
	~Octree();

	inline void setBoundingBox(const AABB &boundingBox) { this->boundingBox = boundingBox; }
	void buildTree(std::list<OctreeData<T>> &data);

	//std::list<OctreeData<T>>& getData(const Frustum &frustum);

private:
	const float MIN_PARTITION_SIZE = 1;
	const unsigned int MAX_DATA_PER_PARTITION = 1;

	std::list<OctreeData<T>> dataList;

	AABB boundingBox;
	Octree* childs[8];
	bool hasChildren;

	void insert(OctreeData<T> &data);

	void split();
	void merge();

	int findAppropriateChild(const AABB &objectBoundingBox) const;
};

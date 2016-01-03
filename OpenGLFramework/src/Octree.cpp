#include "Octree.h"


template <class T>
Octree<T>::Octree(const AABB &rootBoundingBox, const std::list<OctreeData<T>> &data) :
boundingBox(rootBoundingBox), hasChildren(false)
{
	buildTree(data);
}

template <class T>
Octree<T>::Octree(const AABB &rootBoundingBox) : boundingBox(rootBoundingBox), hasChildren(false) {
}

template <class T>
Octree<T>::Octree() : hasChildren(false) {
}

template <class T>
Octree<T>::~Octree() {
	if (hasChildren) {
		for (int i = 0; i < 8; ++i) {
			delete childs[i];
		}
	}
}

template <class T>
void Octree<T>::buildTree(std::list<OctreeData<T>> &data) {
	std::list<OctreeData<T>>::iterator it = data.begin();
	while (it != data.end()) {
		if (boundingBox.containsAABB((*it).boundingBox))
			insert(*it);

		it++;
	}
}

template <class T>
void Octree<T>::insert(OctreeData<T> &data) {
	if (hasChildren) {
		int child = findAppropriateChild(data.boundingBox);
		if (child < 0) {
			dataList.push_back(data); //this is bad. aabb intersects multiple partitions. better solutions needed
		}
		else {
			childs[child]->insert(data);
		}
	}
	else {
		if (dataList.size() < MAX_DATA_PER_PARTITION || boundingBox.getDimensions().x <= MIN_PARTITION_SIZE) {
			dataList.push_back(data);
		}
		else {
			split();

			std::list<OctreeData<T>> toInsertList;
			std::list<OctreeData<T>>::iterator it = dataList.begin();
			while (it != dataList.end()) {
				toInsertList.push_back(*it);
				it++;
			}
			toInsertList.push_back(data);
			dataList.clear();


			//dataList.push_back(data); //put my data and the new data to insert in a single place
			buildTree(toInsertList); //distribute them by the new children
			//dataList.clear();
		}
	}
}

template <class T>
void Octree<T>::split() {
	AABB newAABBs[8];

	glm::vec3 quarterDimension = boundingBox.getDimensions() / 4.0f;
	glm::vec3 halfDimension = boundingBox.getDimensions() / 2.0f;
	glm::vec3 center = boundingBox.getCenter();

	newAABBs[0].setCenterAndDimensions(center + glm::vec3(-quarterDimension.x, quarterDimension.y, quarterDimension.z), halfDimension);
	newAABBs[1].setCenterAndDimensions(center + glm::vec3(quarterDimension.x, quarterDimension.y, quarterDimension.z), halfDimension);
	
	newAABBs[2].setCenterAndDimensions(center + glm::vec3(-quarterDimension.x, quarterDimension.y, -quarterDimension.z), halfDimension);
	newAABBs[3].setCenterAndDimensions(center + glm::vec3(quarterDimension.x, quarterDimension.y, -quarterDimension.z), halfDimension);

	newAABBs[4].setCenterAndDimensions(center + glm::vec3(-quarterDimension.x, -quarterDimension.y, quarterDimension.z), halfDimension);
	newAABBs[5].setCenterAndDimensions(center + glm::vec3(quarterDimension.x, -quarterDimension.y, quarterDimension.z), halfDimension);

	newAABBs[6].setCenterAndDimensions(center + glm::vec3(-quarterDimension.x, -quarterDimension.y, -quarterDimension.z), halfDimension);
	newAABBs[7].setCenterAndDimensions(center + glm::vec3(quarterDimension.x, -quarterDimension.y, -quarterDimension.z), halfDimension);

	for (int i = 0; i < 8; ++i) {
		childs[i] = new Octree(newAABBs[i]);
		hasChildren = true;
	}
}

template <class T>
void Octree<T>::merge() {

}

template <class T>
int Octree<T>::findAppropriateChild(const AABB &objectBoundingBox) const {
	for (int i = 0; i < 8; ++i) {
		if (childs[i]->boundingBox.containsAABB(objectBoundingBox)) {
			return i;
		}
	}
	return -1;
}
#include "Interpolator.h"
#include <glm/glm.hpp>

template class Interpolator<glm::vec3>;

template <class T>
Interpolator<T>::Interpolator() {
}

template <class T>
Interpolator<T>::~Interpolator() {
}

template <class T>
const T& Interpolator<T>::getPointLooped(const std::vector<T> &points, int index) {
	int size = (int)points.size();
	int mod = index % size;
	if (mod < 0) {
		return points[size + mod];
	}
	else {
		return points[mod];
	}
}
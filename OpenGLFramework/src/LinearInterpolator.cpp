#include "LinearInterpolator.h"
#include <glm/glm.hpp>

template class LinearInterpolator<glm::vec3>;

template <class T>
LinearInterpolator<T>::LinearInterpolator() {
}

template <class T>
LinearInterpolator<T>::~LinearInterpolator() {
}

template <class T>
const T& LinearInterpolator<T>::interpolate(const std::vector<T> &points, int basePoint, float value) {
	value = glm::clamp(value, 0.0f, 1.0f);
	temp = getPointLooped(points, basePoint) * (1.0f - value) + getPointLooped(points, basePoint+1) * value;
	return temp;
}
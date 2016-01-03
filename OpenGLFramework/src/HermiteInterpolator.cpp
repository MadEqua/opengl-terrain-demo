#include "HermiteInterpolator.h"
#include <glm/glm.hpp>

template class HermiteInterpolator<glm::vec3>;

template <class T>
HermiteInterpolator<T>::HermiteInterpolator(){
}

template <class T>
HermiteInterpolator<T>::~HermiteInterpolator() {
}

template <class T>
const T& HermiteInterpolator<T>::interpolate(const std::vector<T> &points, int basePoint, float value) {
	value = glm::clamp(value * value * (3.0f - 2.0f * value), 0.0f, 1.0f);
	temp = getPointLooped(points, basePoint) * (1.0f - value) + getPointLooped(points, basePoint + 1) * value;
	return temp;
}
#include "CubicInterpolator.h"
#include <glm/glm.hpp>

template class CubicInterpolator<glm::vec3>;

template <class T>
CubicInterpolator<T>::CubicInterpolator(){
}

template <class T>
CubicInterpolator<T>::~CubicInterpolator() {
}

template <class T>
const T& CubicInterpolator<T>::interpolate(const std::vector<T> &points, int basePoint, float value) {
	value = glm::clamp(value, 0.0f, 1.0f);

	const T &y0 = getPointLooped(points, basePoint - 1);
	const T &y1 = getPointLooped(points, basePoint);
	const T &y2 = getPointLooped(points, basePoint + 1);
	const T &y3 = getPointLooped(points, basePoint + 2);

	float value2 = value * value;
	T a0 = -0.5f * y0 + 1.5f * y1 - 1.5f * y2 + 0.5f * y3;
	T a1 = y0 - 2.5f * y1 + 2.0f * y2 - 0.5f * y3;
	T a2 = -0.5f * y0 + 0.5f * y2;
	T a3 = y1;

	//a0 * x^3 + a1 * x^2 + a2 * x + a3
	temp = a0 * value * value2 + a1 * value2 + a2 * value + a3;
	return temp;
}
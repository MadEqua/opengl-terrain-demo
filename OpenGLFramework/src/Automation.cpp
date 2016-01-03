#include "Automation.h"
#include "LinearInterpolator.h"
#include "HermiteInterpolator.h"
#include "CubicInterpolator.h"
#include <glm/glm.hpp>

template class Automation<glm::vec3>;

template <class T>
Automation<T>::Automation(float duration) : 
	duration(duration), progress(0), interpolator(interpolator) {

	interpolator = new CubicInterpolator<T>(); //TODO: work with different interpolators
}

template <class T>
Automation<T>::~Automation() {
	delete interpolator;
}

template <class T>
const T& Automation<T>::getValue(float deltaTime) {
	float timePerCpPair = duration / (float)(cps.size() - 1.0f);
	float v = progress / timePerCpPair;
	float inter = glm::fract(v);

	int cp1 = (int)glm::floor(v) % cps.size();
	progress += deltaTime;
	return interpolator->interpolate(cps, cp1, inter);
}
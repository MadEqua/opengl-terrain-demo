#pragma once

#include <cmath>
#include <algorithm>
#include <string>
#include <glm/glm.hpp>

using namespace std;

class Utils {
public:
	static const float MAX_FLOAT;
	static const float MIN_FLOAT;
	static const int MAX_INT;
	static const int MIN_INT;
	static const float PI;
	static const float EPSILON_FLOAT;

	inline static float clamp(const float v, const float lo, const float hi) {
		return std::max(lo, std::min(hi, v));
	}

	inline static float degToRad(const float deg) {
		return (PI / 180.0f * deg);
	}

	inline static bool floatEquals(const float value, const float comp, const float epsilon = EPSILON_FLOAT) {
		return abs(value - comp) <= epsilon;
	}

	inline static float clampAngle(const float angle) {
		float x = fmod(angle, 360.0f);
		if (x < 0)
			x += 360;
		return x;
	}

	static void checkGlError(const char *file, int line);
	static void printMatrix(const std::string &preamble, glm::mat4 matrix);
	static void printFloatArrray(const float *array, const int size);
};
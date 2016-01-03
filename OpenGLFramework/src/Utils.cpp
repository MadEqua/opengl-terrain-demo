#include "Utils.h"
#include <limits>
#include <iostream>
#include <GL/glew.h>

const float Utils::MAX_FLOAT = numeric_limits<float>::max();
const float Utils::MIN_FLOAT = numeric_limits<float>::min();
const int Utils::MAX_INT = numeric_limits<int>::max();
const int Utils::MIN_INT = numeric_limits<int>::min();
const float Utils::PI = 3.1415926535897f;
const float Utils::EPSILON_FLOAT = 1e-6f;

void Utils::checkGlError(const char *file, int line) {
	GLenum err(glGetError());

	while (err != GL_NO_ERROR) {
		std::string error;

		switch (err) {
		case GL_INVALID_OPERATION:      error = "INVALID_OPERATION";      break;
		case GL_INVALID_ENUM:           error = "INVALID_ENUM";           break;
		case GL_INVALID_VALUE:          error = "INVALID_VALUE";          break;
		case GL_OUT_OF_MEMORY:          error = "OUT_OF_MEMORY";          break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:  error = "INVALID_FRAMEBUFFER_OPERATION";  break;
		}

		std::cout << "GL_" << error.c_str() << " - " << file << ":" << line << std::endl;
		err = glGetError();
	}
}

void Utils::printMatrix(const std::string &preamble, glm::mat4 matrix) {
	std::cout << preamble << ":\n";
	
	for (int j = 0; j < matrix[0].length(); ++j) {
		std::cout << "| ";
		for (int i = 0; i < matrix.length(); ++i) {
			printf("%+.3f ", matrix[i][j]);
		}
		std::cout << "|\n";
	}

	std::cout << "\n";
}

void Utils::printFloatArrray(const float *array, const int size) {
	for (int i = 0; i < size; ++i) {
		std::cout << array[i];
		if (i < size - 1)
			std::cout << ", ";
	}
	std::cout << "\n";
}
#include "IBO.h"

#include <cstring>

IBO::IBO(unsigned int numberOfIndices) : numberOfIndices(numberOfIndices), actualPointer(0) {
	this->indices = new unsigned int[numberOfIndices];
}

IBO::~IBO() {
	deleteLocalData();
}

void IBO::setIndices(GLuint *indices) {
	memcpy(this->indices, indices, numberOfIndices * sizeof(unsigned int));
}

void IBO::addIndex(GLuint index) {
	indices[actualPointer++] = index;
}

void IBO::bind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, name);
}

void IBO::pushToGPU(bool deleteAfterPush) {
	sizeInBytes = numberOfIndices * sizeof(unsigned int);
	glNamedBufferStorage(name, sizeInBytes, indices, 0);

	if (deleteAfterPush)
		deleteLocalData();
}

void IBO::deleteLocalData() {
	if (this->indices != 0) {
		delete[] this->indices;
		this->indices = 0;
	}
}
#include "GLBuffer.h"

GLBuffer::GLBuffer() : sizeInBytes(0) {
	create();
}

GLBuffer::~GLBuffer() {
	destroy();
}

void GLBuffer::create() {
	glCreateBuffers(1, &name);
}


void GLBuffer::destroy() {
	glDeleteBuffers(1, &name);
}
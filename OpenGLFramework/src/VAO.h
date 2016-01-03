#pragma once

#include "GLObject.h"

class VAO : public GLObject
{
public:
	VAO();
	~VAO();

	virtual void create();
	virtual void destroy();

	virtual void bind() const;
	virtual void pushToGPU(bool deleteAfterPush = true) {};

	void enableVertexAttribute(GLuint attributeIndex, GLuint vbo, GLintptr VBOoffset, GLsizei VBOstride);
	void setVertexAttributeFormat(GLuint attributeIndex, GLint size, GLenum type, GLboolean normalized, GLuint relativeOffset);
};


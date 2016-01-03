#pragma once

#include <GL/glew.h>
#include "GLBuffer.h"


class IBO : public GLBuffer
{
public:
	IBO(unsigned int indicesCount);
	~IBO();

	void setIndices(GLuint *indices);
	void addIndex(GLuint index);

	virtual void bind() const;
	virtual void pushToGPU(bool deleteAfterPush = true);

	inline unsigned int getNumberOfIndices() const { return numberOfIndices; }

private:
	GLuint *indices;

	unsigned int actualPointer;
	unsigned int numberOfIndices;

	void deleteLocalData();
};


#pragma once

#include "GLObject.h"

class GLBuffer : public GLObject
{
public:
	GLBuffer();
	~GLBuffer();

	virtual void create();
	virtual void destroy();

	inline unsigned int getSizeInBytes() const { return sizeInBytes; }

protected:
	unsigned int sizeInBytes;
};


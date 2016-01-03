#pragma once

#include "Texture.h"

class Texture1D : public Texture
{
public:
	Texture1D();
	~Texture1D();

	virtual void pushToGPU(bool deleteAfterPush = false);
};


#pragma once

#include "Texture.h"
#include <vector>

class Texture2DArray : public Texture
{
public:
	Texture2DArray();
	virtual ~Texture2DArray();

	void pushToGPU(bool deleteAfterPush = false);
};


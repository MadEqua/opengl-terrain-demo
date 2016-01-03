#pragma once

#include "Texture.h"

class TextureCube : public Texture
{
public:
	TextureCube(bool invertYFaces = false); //prefix 0 = +x, 1 = -x, +y, -y, +z, -z
	~TextureCube();

	virtual void initFromFile(const std::string &path, const ImageFormat &format = ImageFormat::IMAGE_FORMAT_SRGB8);
	virtual void pushToGPU(bool deleteAfterPush = false);

private:
	bool invertYFaces;
};


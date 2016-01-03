#pragma once

#include "Asset.h"
#include "GLObject.h"
#include <GL/glew.h>
#include <IL/il.h>

#include <cstdint>
#include <string>
#include <vector>

#include <glm/glm.hpp>

struct ImageFormat {
	GLenum sizedFormat, baseFormat, type;
	uint32_t numberOfChannels, bytesPerChannel;

	const static ImageFormat IMAGE_FORMAT_SRGBA8;
	const static ImageFormat IMAGE_FORMAT_SRGB8;

	const static ImageFormat IMAGE_FORMAT_SBGR8;

	const static ImageFormat IMAGE_FORMAT_RGBA8_LINEAR;
	const static ImageFormat IMAGE_FORMAT_RGB8_LINEAR;

	const static ImageFormat IMAGE_FORMAT_R8;
	const static ImageFormat IMAGE_FORMAT_R16;

	const static ImageFormat IMAGE_FORMAT_DEPTH_32F;

	ImageFormat(GLenum sizedFormat, GLenum baseFormat, GLenum type, uint32_t numberOfChannels, uint32_t bytesPerChannel) :
		sizedFormat(sizedFormat), baseFormat(baseFormat), type(type), numberOfChannels(numberOfChannels), bytesPerChannel(bytesPerChannel) {
	}

	ImageFormat() {}
};

struct ImageData {
	ImageFormat format;
	uint32_t width, height;
	uint8_t* data;
	uint32_t sizeInBytes;
};


//TODO: allow binding of different Samplers, to be able to reutilize the same texture data with different parameters
class Texture : public GLObject, public Asset
{
public:
	Texture(GLenum type);
	virtual ~Texture();

	virtual void create();
	virtual void destroy();

	virtual void bind() const;
	virtual void pushToGPU(bool deleteAfterPush = false) = 0;

	void bind(int unit) const;
	void changeTextureParameter(GLenum parameter, GLfloat value);

	virtual const void* getTexel(const ImageData &imageData, const int x, const int y, const int z);
	virtual void setTexel(int imageIndex, const int x, const int y, const int z, const void* value);

	virtual void initFromFile(const std::string &path, const ImageFormat &format = ImageFormat::IMAGE_FORMAT_SRGB8);
	virtual void initFromFiles(const std::vector<std::string> &paths, const ImageFormat &format = ImageFormat::IMAGE_FORMAT_SRGB8);

	void initFromData(uint32_t width, uint32_t height, const uint8_t* data, const ImageFormat &format = ImageFormat::IMAGE_FORMAT_SRGB8);

	inline void setMipmapLevels(int levels) { mipmapLevels = levels; }

	inline const ImageData& getImageData(int index) { return images[index]; }

protected:
	GLenum type;
	const ImageData loadFile(const std::string &path, const ImageFormat &format);

	int mipmapLevels;

	std::vector<ImageData> images;
};
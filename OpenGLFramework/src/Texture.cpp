#include "Texture.h"

#include <iostream>

const ImageFormat ImageFormat::IMAGE_FORMAT_SRGBA8 = ImageFormat(GL_SRGB8_ALPHA8, GL_RGBA, GL_UNSIGNED_BYTE, 4, 1);
const ImageFormat ImageFormat::IMAGE_FORMAT_SRGB8 = ImageFormat(GL_SRGB8, GL_RGB, GL_UNSIGNED_BYTE, 3, 1);

const ImageFormat ImageFormat::IMAGE_FORMAT_SBGR8 = ImageFormat(GL_SRGB8, GL_BGR, GL_UNSIGNED_BYTE, 3, 1);

const ImageFormat ImageFormat::IMAGE_FORMAT_RGBA8_LINEAR = ImageFormat(GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, 4, 1);
const ImageFormat ImageFormat::IMAGE_FORMAT_RGB8_LINEAR = ImageFormat(GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE, 3, 1);

const ImageFormat ImageFormat::IMAGE_FORMAT_R8 = ImageFormat(GL_R8, GL_RED, GL_UNSIGNED_BYTE, 1, 1);
const ImageFormat ImageFormat::IMAGE_FORMAT_R16 = ImageFormat(GL_R16, GL_RED, GL_UNSIGNED_SHORT, 1, 2);

const ImageFormat ImageFormat::IMAGE_FORMAT_DEPTH_32F = ImageFormat(GL_DEPTH_COMPONENT32F, GL_DEPTH_COMPONENT, GL_FLOAT, 1, 4);

Texture::Texture(GLenum type) : type(type), mipmapLevels(8) {
	create();
}

Texture::~Texture() {
	destroy();

	for (unsigned int i = 0; i < images.size(); ++i) {
		if (images[i].data != 0)
			delete[] images[i].data;
	}
}

void Texture::create() {
	glCreateTextures(type, 1, &name);
}

void Texture::destroy() {
	glDeleteTextures(1, &name);
}

void Texture::changeTextureParameter(GLenum parameter, GLfloat value) {
	glTextureParameterf(name, parameter, value);
}

void Texture::bind(int unit) const {
	glBindTextureUnit(unit, name);
}

void Texture::bind() const {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(type, name);
}

void Texture::initFromFile(const std::string &path, const ImageFormat &format) {
	images.push_back(loadFile(path, format));
	pushToGPU();
}

void Texture::initFromFiles(const std::vector<std::string> &paths, const ImageFormat &format) {
	for (std::string path : paths) {
		images.push_back(loadFile(path, format));
	}
	pushToGPU();
}

void Texture::initFromData(uint32_t width, uint32_t height, const uint8_t* data, const ImageFormat &format) {

	ImageData imageData;
	imageData.format = format;

	imageData.width = width;
	imageData.height = height;
	imageData.sizeInBytes = width * height * format.numberOfChannels * format.bytesPerChannel;

	if (data != 0) {
		imageData.data = new uint8_t[imageData.sizeInBytes];
		memcpy(imageData.data, data, imageData.sizeInBytes);
	}
	else
		imageData.data = 0;

	images.push_back(imageData);

	pushToGPU();
}

const ImageData Texture::loadFile(const std::string &path, const ImageFormat &format) {
	ilInit();

	ILuint image;
	ilGenImages(1, &image);
	ilBindImage(image);
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

	ImageData imageData;

	ILboolean result = ilLoadImage(path.c_str());
	if (result) {
		//ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

		imageData.format = format;

		imageData.width = ilGetInteger(IL_IMAGE_WIDTH);
		imageData.height = ilGetInteger(IL_IMAGE_HEIGHT);
		imageData.sizeInBytes = imageData.width * imageData.height * imageData.format.numberOfChannels * imageData.format.bytesPerChannel;

		imageData.data = new uint8_t[imageData.sizeInBytes];
		memcpy(imageData.data, ilGetData(), imageData.sizeInBytes);
	}
	else {
		std::cout << "Failed to load image from file: " << path << std::endl;
	}

	ilDeleteImages(1, &image);

	return imageData;
}

const void* Texture::getTexel(const ImageData &imageData, const int x, const int y, const int z) {
	uint32_t texelSize = imageData.format.bytesPerChannel * imageData.format.numberOfChannels;
	uint8_t *ptr = imageData.data + (y * texelSize * imageData.width) + (x * texelSize);
	return ptr;
}

void Texture::setTexel(int imageIndex, const int x, const int y, const int z, const void *value) {
	const ImageData &imageData = images[imageIndex];
	uint32_t texelSize = imageData.format.bytesPerChannel * imageData.format.numberOfChannels;
	uint8_t *ptr = imageData.data + (y * texelSize * imageData.width) + (x * texelSize);

	memcpy(ptr, value, texelSize);
}
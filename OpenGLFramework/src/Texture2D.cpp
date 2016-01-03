#include "Texture2D.h"

#include <GL\glew.h>

Texture2D::Texture2D() : Texture(GL_TEXTURE_2D) {
}

Texture2D::~Texture2D() {
}

void Texture2D::pushToGPU(bool deleteAfterPush) {
	ImageData &imageData = images[0];

	glTextureStorage2D(name, mipmapLevels, imageData.format.sizedFormat, imageData.width, imageData.height); //allocate space for all

	if (imageData.data != 0)
		glTextureSubImage2D(name, 0, 0, 0, imageData.width, imageData.height, imageData.format.baseFormat, imageData.format.type, imageData.data); //level 0 mipmap

	if (mipmapLevels > 1) {
		glGenerateTextureMipmap(name); //generate the remaining mipmaps
		glTextureParameterf(name, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}
	else
		glTextureParameterf(name, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTextureParameterf(name, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTextureParameterf(name, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameterf(name, GL_TEXTURE_WRAP_T, GL_REPEAT);

	if (deleteAfterPush) {
		delete[] imageData.data;
		imageData.data = 0;
	}
}

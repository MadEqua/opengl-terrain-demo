#include "TextureCube.h"
#include <GL\glew.h>
#include <IL/il.h>

TextureCube::TextureCube(bool invertYFaces) :
Texture(GL_TEXTURE_CUBE_MAP), invertYFaces(invertYFaces) {

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}

TextureCube::~TextureCube() {
}

void TextureCube::initFromFile(const std::string &path, const ImageFormat &format) {
	for (int i = 0; i < 6; ++i) {
		std::string extension = path.substr(path.length() - 3, 3);
		std::string noExtension = path.substr(0, path.length() - 4);
		std::string newPath = "";

		int n = i;
		if (invertYFaces) {

			if (i == 2)
				n = 3;
			else if (i == 3)
				n = 2;
		}

		newPath.append(noExtension).append(std::to_string(n)).append(".").append(extension);
		images.push_back(loadFile(newPath, format));
	}

	pushToGPU();
}

void TextureCube::pushToGPU(bool deleteAfterPush) {
	glTextureStorage2D(name, mipmapLevels, images[0].format.sizedFormat, images[0].width, images[0].height); //allocate space for all
	bind();

	for (int i = 0; i < 6; ++i) {
		ImageData &imageData = images[i];

		if (imageData.data != 0)
			glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, 0, 0, imageData.width, imageData.height, imageData.format.baseFormat, imageData.format.type, imageData.data); //level 0 mipmap

		if (deleteAfterPush) {
			delete[] imageData.data;
			imageData.data = 0;
		}
	}

	if (mipmapLevels > 1) {
		glGenerateTextureMipmap(name); //generate the remaining mipmaps
		glTextureParameterf(name, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}
	else
		glTextureParameterf(name, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTextureParameterf(name, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTextureParameterf(name, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameterf(name, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTextureParameterf(name, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}
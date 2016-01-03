#include "Material.h"

Material::Material() : activeShader(0) {
}

Material::~Material() {
}

void Material::setShaderProgram(unsigned int index, ShaderProgram *shaderProgram) {
	if (index >= shaderPrograms.size())
		shaderPrograms.resize(index + 1);
	shaderPrograms[index] = shaderProgram;
}

void Material::setTexture(unsigned int index, Texture *texture) {
	if (index >= textures.size()) 
		textures.resize(index + 1);  
	textures[index] = texture; 
}

void Material::bind() const {
	bindShader();
	bindTextures();
}

void Material::bindTextures() const {
	for (unsigned int i = 0; i < textures.size(); ++i) {
		textures[i]->bind(i);
	}
}

void Material::bindShader() const {
	shaderPrograms[activeShader]->bind();
}
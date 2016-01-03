#pragma once

#include "Texture.h"
#include "ShaderProgram.h"
#include "Asset.h"

#include <vector>

class Material : public Asset
{
public:
	Material();
	virtual ~Material();

	void setShaderProgram(unsigned int index, ShaderProgram *shaderProgram);
	void setTexture(unsigned int index, Texture *texture);

	inline ShaderProgram* getShaderProgram(int id) const { return shaderPrograms[id]; }
	inline Texture* getTexture(int id) const { return textures[id]; }

	inline int getTextureCount() { return textures.size(); }
	inline void setActiveShader(int s) { activeShader = s; }

	void bind() const;
	void bindTextures() const;
	void bindShader() const;

private:
	std::vector<ShaderProgram*> shaderPrograms;
	std::vector<Texture*> textures;

	int activeShader;
};
#pragma once

#include "SceneObject.h"

#include <string>

#include "Shader.h"
#include "ShaderProgram.h"
#include "Mesh.h"
#include "TextureCube.h"
#include "Material.h"

//TODO: assumes only one SkyBox on the scene and adds its assets to the Manager without checks
class SkyBox : public SceneObject
{
public:
	SkyBox(const std::string &texturePath);
	~SkyBox();

	virtual void initialize();
	virtual void render(int passID);
	virtual void update(double deltaTime);

private:
	std::string texturePath;
};


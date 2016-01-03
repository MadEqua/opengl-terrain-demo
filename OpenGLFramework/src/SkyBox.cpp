#include "SkyBox.h"

#include "AssetManager.h"
#include "TextureCube.h"
#include "ShaderProgram.h"
#include "Shader.h"
#include "Mesh.h"
#include "Application.h"
#include "Camera.h"
#include <vector>
#include <glm/gtc/type_ptr.hpp>


SkyBox::SkyBox(const std::string &texturePath) : texturePath(texturePath) {
	Shader *shaderV = new Shader("shaders/skybox.vs");
	Shader  *shaderF = new Shader("shaders/skybox.fs");

	std::vector<Shader*> skyShaders;
	skyShaders.push_back(shaderV);
	skyShaders.push_back(shaderF);
	ShaderProgram *shaderProgram = new ShaderProgram(skyShaders);

	TextureCube *texture = new TextureCube(true);
	texture->setMipmapLevels(1);
	texture->initFromFile(texturePath);

	Material *material = new Material();
	material->setTexture(0, texture);
	material->setShaderProgram(0, shaderProgram);

	Mesh *mesh = new Mesh();
	mesh->initFromRawData(8, 36);
	const float v = 2.0f;
	GLfloat positions[] = {
		-v, v, v,
		-v, -v, v,
		v, -v, v,
		v, v, v,
		-v, v, -v,
		-v, -v, -v,
		v, -v, -v,
		v, v, -v,
	};
	GLuint indices[] = {
		2, 1, 0,
		0, 3, 2,
		4, 5, 6,
		6, 7, 4,
		6, 2, 7,
		2, 3, 7,
		4, 1, 5,
		4, 0, 1,
		5, 1, 2,
		5, 2, 6,
		4, 3, 0,
		4, 7, 3
	};
	mesh->setAttributeData(0, positions);
	mesh->setIndices(indices);
	mesh->endFromRawData();

	setMaterial(material);
	setMesh(mesh);

	AssetManager &assetManager = AssetManager::getInstance();
	assetManager.addAsset("skyBoxVertex", shaderV);
	assetManager.addAsset("skyBoxFragment", shaderF);
	assetManager.addAsset("skyBoxShaderProgram", shaderProgram);
	assetManager.addAsset("skyBoxTexture", texture);
	assetManager.addAsset("skyBoxMaterial", material);
	assetManager.addAsset("skyBoxMesh", mesh);
}

SkyBox::~SkyBox() {
}

void SkyBox::initialize() {

}

void SkyBox::render(int passID) {
	material->bind();

	glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(application->getActiveCamera().getProjectionMatrix() *
		application->getActiveCamera().getViewMatrix() *
		transform.getTransformationMatrix()));

	glDisable(GL_DEPTH_TEST);
	//glDepthMask(GL_FALSE);
	mesh->renderIndexed();
	//glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
}

void SkyBox::update(double deltaTime) {
	Camera &camera = application->getActiveCamera();
	if (camera.changedSinceLastCall())
		transform.setPosition(camera.getTransform().getPosition(), WORLD);
}
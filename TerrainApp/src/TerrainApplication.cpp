#include "TerrainApplication.h"
#include "AssetManager.h"
#include "Terrain.h"
#include "SkyBox.h"
#include "Utils.h"
#include "Texture2DArray.h"
#include "TextureCube.h"
#include "Texture2D.h"
#include "Sun.h"
#include "FreeCamera.h"
#include "LookAtCamera.h"

#include <iostream>

TerrainApplication::TerrainApplication() {
	depthFBO = 0;
}

TerrainApplication::~TerrainApplication() {
	delete depthFBO;
	delete cameraPositionAutomation;
	delete cameraLookAtAutomation;
}

void TerrainApplication::initialize() {
	setWindowProperties("OpenGL Terrain Demo", 1280, 800);

	AssetManager &assetManager = AssetManager::getInstance();
	
	//SKYBOX
	SkyBox *skyBox = new SkyBox("textures/sky/sky.png");
	skyID = addSceneObject(skyBox);

	//SUN
	Mesh *sunMesh = new Mesh();
	sunMesh->initFromFile("meshes/sphere.obj");
	assetManager.addAsset("sphereMesh", sunMesh);

	Shader *sunV = new Shader("shaders/white.vs");
	Shader *sunF = new Shader("shaders/white.fs");
	assetManager.addAsset("sunV", sunV);
	assetManager.addAsset("sunF", sunF);

	std::vector<Shader*> shaders;
	shaders.push_back(sunV);
	shaders.push_back(sunF);
	ShaderProgram *sunShaderProgram = new ShaderProgram(shaders);
	assetManager.addAsset("sunShaderProgram", sunShaderProgram);

	Material *sunMaterial = new Material();
	sunMaterial->setShaderProgram(0, sunShaderProgram);
	assetManager.addAsset("sunMaterial", sunMaterial);

	Sun *sun = new Sun(-1.0f, -0.6f, 0.0f);
	sun->setMesh(sunMesh);
	sun->setMaterial(sunMaterial);
	sunID = addSceneObject(sun);
	addEventListener(sun);

	//TERRAIN
	Shader *terrainVertex = new Shader("shaders/terrain.vs");
	Shader *terrainTessControl = new Shader("shaders/terrain.tcs");
	Shader *terrainTessEval = new Shader("shaders/terrain.tes");
	Shader *terrainFragment = new Shader("shaders/terrain.fs");
	Shader *terrainFragment2 = new Shader("shaders/terrain2.fs");
	Shader *terrainGeometry = new Shader("shaders/terrain.gs");
	
	std::vector<Shader*> terrainShaders1;
	terrainShaders1.push_back(terrainVertex);
	terrainShaders1.push_back(terrainFragment);
	terrainShaders1.push_back(terrainTessControl);
	terrainShaders1.push_back(terrainTessEval);
	terrainShaders1.push_back(terrainGeometry);
	ShaderProgram *terrainShaderProgram1 = new ShaderProgram(terrainShaders1);

	std::vector<Shader*> terrainShaders2;
	terrainShaders2.push_back(terrainVertex);
	terrainShaders2.push_back(terrainFragment2);
	terrainShaders2.push_back(terrainTessControl);
	terrainShaders2.push_back(terrainTessEval);
	terrainShaders2.push_back(terrainGeometry);
	ShaderProgram *terrainShaderProgram2 = new ShaderProgram(terrainShaders2);

	assetManager.addAsset("terrainVertex", terrainVertex);
	assetManager.addAsset("terrainFragment", terrainFragment);
	assetManager.addAsset("terrainFragment2", terrainFragment2);
	assetManager.addAsset("terrainTessControl", terrainTessControl);
	assetManager.addAsset("terrainTessEval", terrainTessEval);
	assetManager.addAsset("terrainGeometry", terrainGeometry);
	assetManager.addAsset("terrainShaderProgram1", terrainShaderProgram1);
	assetManager.addAsset("terrainShaderProgram2", terrainShaderProgram2);

	//depth pass
	Shader *terrainDepthVertex = new Shader("shaders/terrainDepthPass.vs");
	Shader *terrainDepthTessControl = new Shader("shaders/terrainDepthPass.tcs");
	Shader *terrainDepthTessEval = new Shader("shaders/terrainDepthPass.tes");
	Shader *terrainDepthFragment = new Shader("shaders/terrainDepthPass.fs");
	
	std::vector<Shader*> terrainDepthShaders;
	terrainDepthShaders.push_back(terrainDepthVertex);
	terrainDepthShaders.push_back(terrainDepthFragment);
	terrainDepthShaders.push_back(terrainDepthTessControl);
	terrainDepthShaders.push_back(terrainDepthTessEval);
	ShaderProgram *terrainDepthShaderProgram = new ShaderProgram(terrainDepthShaders);

	assetManager.addAsset("terrainDepthVertex", terrainDepthVertex);
	assetManager.addAsset("terrainDepthFragment", terrainDepthFragment);
	assetManager.addAsset("terrainDepthTessControl", terrainDepthTessControl);
	assetManager.addAsset("terrainDepthTessEval", terrainDepthTessEval);
	assetManager.addAsset("terrainDepthShaderProgram", terrainDepthShaderProgram);
	

	Texture2D *terrainDisplacement1 = new Texture2D();
	terrainDisplacement1->initFromFile("textures/height16bit1.png", ImageFormat::IMAGE_FORMAT_R16);
	terrainDisplacement1->changeTextureParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	terrainDisplacement1->changeTextureParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	assetManager.addAsset("terrainDisplacement1", terrainDisplacement1);

	Texture2D *terrainDisplacement2 = new Texture2D();
	terrainDisplacement2->initFromFile("textures/height16bit2.png", ImageFormat::IMAGE_FORMAT_R16);
	terrainDisplacement2->changeTextureParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	terrainDisplacement2->changeTextureParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	assetManager.addAsset("terrainDisplacement2", terrainDisplacement2);

	Texture2D *terrainDisplacement3 = new Texture2D();
	terrainDisplacement3->initFromFile("textures/height16bit3.png", ImageFormat::IMAGE_FORMAT_R16);
	terrainDisplacement3->changeTextureParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	terrainDisplacement3->changeTextureParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	assetManager.addAsset("terrainDisplacement3", terrainDisplacement3);

	Texture2D *terrainNormals1 = new Texture2D();
	terrainNormals1->initFromFile("textures/normals1.png", ImageFormat::IMAGE_FORMAT_RGB8_LINEAR);
	terrainNormals1->changeTextureParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	terrainNormals1->changeTextureParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	assetManager.addAsset("terrainNormals1", terrainNormals1);

	Texture2D *terrainNormals2 = new Texture2D();
	terrainNormals2->initFromFile("textures/normals2.png", ImageFormat::IMAGE_FORMAT_RGB8_LINEAR);
	terrainNormals2->changeTextureParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	terrainNormals2->changeTextureParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	assetManager.addAsset("terrainNormals2", terrainNormals2);

	Texture2D *terrainNormals3 = new Texture2D();
	terrainNormals3->initFromFile("textures/normals3.png", ImageFormat::IMAGE_FORMAT_RGB8_LINEAR);
	terrainNormals3->changeTextureParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	terrainNormals3->changeTextureParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	assetManager.addAsset("terrainNormals3", terrainNormals3);

	std::vector<std::string> terrainTextures1;
	terrainTextures1.push_back("textures/1.png");
	terrainTextures1.push_back("textures/2.jpg");
	terrainTextures1.push_back("textures/3.jpg");
	terrainTextures1.push_back("textures/4.jpg");
	Texture2DArray *terrainTexturesArray1 = new Texture2DArray();
	terrainTexturesArray1->initFromFiles(terrainTextures1);
	assetManager.addAsset("terrainTexturesArray1", terrainTexturesArray1);

	std::vector<std::string> terrainTextures2;
	terrainTextures2.push_back("textures/6.jpg");
	terrainTextures2.push_back("textures/2.jpg");
	terrainTextures2.push_back("textures/8.jpg");
	terrainTextures2.push_back("textures/1.png");
	Texture2DArray *terrainTexturesArray2 = new Texture2DArray();
	terrainTexturesArray2->initFromFiles(terrainTextures2);
	assetManager.addAsset("terrainTexturesArray2", terrainTexturesArray2);


	Material *terrainMaterial1 = new Material();
	terrainMaterial1->setShaderProgram(0, terrainShaderProgram1);
	terrainMaterial1->setShaderProgram(1, terrainShaderProgram2);
	terrainMaterial1->setShaderProgram(2, terrainDepthShaderProgram);
	terrainMaterial1->setTexture(0, terrainDisplacement1);
	terrainMaterial1->setTexture(1, terrainNormals1);
	terrainMaterial1->setTexture(2, terrainTexturesArray1);
	assetManager.addAsset("terrainMaterial1", terrainMaterial1);

	Material *terrainMaterial2 = new Material();
	terrainMaterial2->setShaderProgram(0, terrainShaderProgram1);
	terrainMaterial2->setShaderProgram(1, terrainShaderProgram2);
	terrainMaterial2->setShaderProgram(2, terrainDepthShaderProgram);
	terrainMaterial2->setTexture(0, terrainDisplacement2);
	terrainMaterial2->setTexture(1, terrainNormals2);
	terrainMaterial2->setTexture(2, terrainTexturesArray2);
	assetManager.addAsset("terrainMaterial2", terrainMaterial2);

	Material *terrainMaterial3 = new Material();
	terrainMaterial3->setShaderProgram(0, terrainShaderProgram1);
	terrainMaterial3->setShaderProgram(1, terrainShaderProgram2);
	terrainMaterial3->setShaderProgram(2, terrainDepthShaderProgram);
	terrainMaterial3->setTexture(0, terrainDisplacement3);
	terrainMaterial3->setTexture(1, terrainNormals3);
	terrainMaterial3->setTexture(2, terrainTexturesArray1);
	assetManager.addAsset("terrainMaterial3", terrainMaterial3);

	Terrain *terrain = new Terrain(sun);
	terrain->setMaterial(terrainMaterial1);

	addEventListener(terrain);
	terrainID = addSceneObject(terrain);

	//DEPTH FBO
	initFBO();
	enableShadowMapping = true;

	//CAMERA
	//FreeCamera *camera = new FreeCamera();
	LookAtCamera *camera = new LookAtCamera();
	addCamera(camera);
	setActiveCamera(0);
	camera->setProjectionMatrix(45.0f, (float)windowWidth / (float)windowHeight, 1.0f, 2048.0f);
	//camera->setPositionAndRotation(glm::vec3(0, 500, 0), glm::vec3(-65, 0, 0));
	camera->setPositionAndLookAt(glm::vec3(5.0f, 500.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	cameraPositionAutomation = new Automation<glm::vec3>(60.0f);
	const float L = -400.0f;
	const float R = 400.0f;
	const float T = 400.0f;
	const float B = -400.0f;
	const float C = 0.0f;
	cameraPositionAutomation->addCheckPoint(glm::vec3(L, 200, B));
	cameraPositionAutomation->addCheckPoint(glm::vec3(C, 300, C));
	cameraPositionAutomation->addCheckPoint(glm::vec3(R, 150, T));
	cameraPositionAutomation->addCheckPoint(glm::vec3(R, 300, C));
	cameraPositionAutomation->addCheckPoint(glm::vec3(L, 250, T));
	cameraPositionAutomation->addCheckPoint(glm::vec3(L, 400, C));
	cameraPositionAutomation->addCheckPoint(glm::vec3(R, 275, B));
	cameraPositionAutomation->addCheckPoint(glm::vec3(C, 100, T));

	cameraLookAtAutomation = new Automation<glm::vec3>(25.0f);
	cameraLookAtAutomation->addCheckPoint(glm::vec3(0, 0, 0));
	cameraLookAtAutomation->addCheckPoint(glm::vec3(400, 25, 0));
	cameraLookAtAutomation->addCheckPoint(glm::vec3(-200, 50, 300));
	cameraLookAtAutomation->addCheckPoint(glm::vec3(0, 75, -100));

	//MISC
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glPolygonOffset(7.5f, 15.0f);

	Application::initialize();
}

void TerrainApplication::initFBO() {
	Texture2D *depthTexture = AssetManager::getInstance().getAsset<Texture2D>("depthTexture");

	if (depthTexture != 0)
		delete depthTexture;

	depthTexture = new Texture2D();
	depthTexture->setMipmapLevels(1);
	depthTexture->initFromData(getWindowWidth()/2, getWindowHeight()/2, 0, ImageFormat::IMAGE_FORMAT_DEPTH_32F);
	//Only for depth texture formats
	depthTexture->changeTextureParameter(GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	depthTexture->changeTextureParameter(GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	Material *m = AssetManager::getInstance().getAsset<Material>("terrainMaterial1");
	m->setTexture(3, depthTexture);
	m = AssetManager::getInstance().getAsset<Material>("terrainMaterial2");
	m->setTexture(3, depthTexture);
	m = AssetManager::getInstance().getAsset<Material>("terrainMaterial3");
	m->setTexture(3, depthTexture);

	AssetManager::getInstance().addAsset("depthTexture", depthTexture);

	if (depthFBO == 0)
		depthFBO = new FBO();

	depthFBO->attachTextureToDepthBuffer(*depthTexture);
	depthFBO->pushToGPU();
}

void TerrainApplication::render() {
	static const GLfloat bgColor[4] = { 0.7f, 0.7f, 0.7f, 1.0f };
	static const GLfloat depthValue = 1.0f;

	if (enableShadowMapping) {
		depthFBO->bind();

		glEnable(GL_POLYGON_OFFSET_FILL);
		glClearBufferfv(GL_DEPTH, 0, &depthValue);
		sceneObjects[terrainID]->render(1);
		glDisable(GL_POLYGON_OFFSET_FILL);

		depthFBO->bindDefaultFramebuffer();
	}
	//glClearBufferfv(GL_COLOR, 0, bgColor);
	glClearBufferfi(GL_DEPTH_STENCIL, 0, 1.0f, 0);

	Application::render();
}

void TerrainApplication::update(double deltaTime) {
	LookAtCamera &camera = ((LookAtCamera&)getActiveCamera());
	const glm::vec3 &pos = cameraPositionAutomation->getValue((float)deltaTime);
	camera.setPosition(pos.x, pos.y, pos.z);

	const glm::vec3 &la = cameraLookAtAutomation->getValue((float)deltaTime);
	camera.setLookAt(la.x, la.y, la.z);

	Application::update(deltaTime);
}

void TerrainApplication::windowResizedCallback(const int width, const int height) {
	Application::windowResizedCallback(width, height);
	//glUniformMatrix4fv(13, 1, GL_FALSE, glm::value_ptr(camera.getProjectionMatrix()));

	if (initialized)
		initFBO();
}

void TerrainApplication::keyCallback(const int key, const int scanCode, const int action, const int mods) {
	Application::keyCallback(key, scanCode, action, mods);
	//std::cout << key << ", " << scanCode << ", " << action << ", " << mods << std::endl;

	if (key == GLFW_KEY_H && action == GLFW_PRESS) {
		enableShadowMapping = !enableShadowMapping;
	}
}

void TerrainApplication::cursorPositionCallback(const double xpos, const double ypos) {
	Application::cursorPositionCallback(xpos, ypos);
	//std::cout << xpos << ", " << ypos << std::endl;
	//glfwSetCursorPos(window, windowWidth / 2, windowHeight / 2);
}

void TerrainApplication::mouseButtonCallback(const int button, const int action, const int mods) {
	Application::mouseButtonCallback(button, action, mods);
	//std::cout << button << ", " << action << ", " << mods << std::endl;
}
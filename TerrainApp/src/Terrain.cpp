#include "Terrain.h"

#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

#include <glm/glm.hpp>

#include "TerrainApplication.h"

#include "AssetManager.h"

Terrain::Terrain(Sun* sun) : sun(sun) {
}

Terrain::~Terrain() {
}

void Terrain::initialize() {
	glPatchParameteri(GL_PATCH_VERTICES, 4);

	displacementScale = 150.0f;
	tessellatedTriWidth = 20.0f;
	gridsize = glm::ivec2(64, 64);
	size = glm::vec2(1000.0f, 1000.0f);  //1km
	patchCount = gridsize.x * gridsize.y;

	camera = &application->getActiveCamera();

	sunProjectionMatrix = glm::ortho(
		-size.x / 2.0f, size.x / 2.0f,
		-size.y / 2.0f, size.y / 2.0f,
		sun->DISTANCE - size.x / 2.0f, sun->DISTANCE + size.x / 2.0f);

	bias = glm::mat4(glm::vec4(0.5f, 0.0f, 0.0f, 0.0f),
		glm::vec4(0.0f, 0.5f, 0.0f, 0.0f),
		glm::vec4(0.0f, 0.0f, 0.5f, 0.0f),
		glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));

	drawWireframe = false;
	enableFog = false;
	heightBasedTexturing = true;

	topViewMatrix = glm::lookAt(glm::vec3(0, 1300.0f, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, -1));
}


void Terrain::update(double deltaTime) {
}

void Terrain::render(int passID) {
	glm::mat4 sunMVP = sunProjectionMatrix * sun->getTransform().getInverseTransformationMatrix();

	int width = application->getWindowWidth();
	int height = application->getWindowHeight();

	if (passID == 0) {

		material->setActiveShader(heightBasedTexturing ? 0 : 1);
		material->bind();

		glUniform2i(21, gridsize.x, gridsize.y);
		glUniform1f(24, displacementScale);
		glUniform2f(25, size.x, size.y);

		glUniform1f(22, tessellatedTriWidth);
		glUniform1i(26, drawWireframe);
		glUniform1i(27, ((TerrainApplication*)application)->enableShadowMapping);
		glUniform1i(28, enableFog);

		glUniformMatrix4fv(11, 1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));
		glUniformMatrix4fv(13, 1, GL_FALSE, glm::value_ptr(camera->getProjectionMatrix()));

		glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(bias * sunMVP));

		glUniform4fv(14, 6, camera->getFrustum().getPackedPlaneData());
		glUniform2i(23, width, height);

		glUniform3fv(30, 1, (GLfloat*)&sun->getDirectionalLight().getDirection());
		glUniform3fv(31, 1, (GLfloat*)&sun->getDirectionalLight().getColor());

		glDrawArraysInstanced(GL_PATCHES, 0, 4, patchCount);


		//TOP CAMERA FOR FRUSTUM CULLING VIEW
		glDisable(GL_DEPTH_TEST);
		int h = int((float)height / 3.0f);
		int w = int((float)h * (float)camera->getFrustum().aspectRatio);
		int x = int((float)width - (float)w - 10.0f);
		int y = 10;

		glViewport(x, y, w, h);

		glUniformMatrix4fv(11, 1, GL_FALSE, glm::value_ptr(topViewMatrix));
		glUniform2i(23, w, h);
		glDrawArraysInstanced(GL_PATCHES, 0, 4, patchCount);

		glEnable(GL_DEPTH_TEST);
		glViewport(0, 0, width, height);
	}
	else {
		//SHADOW MAP
		material->setActiveShader(2);
		material->bind();

		glUniformMatrix4fv(11, 1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));
		glUniformMatrix4fv(13, 1, GL_FALSE, glm::value_ptr(camera->getProjectionMatrix()));
		glUniform1f(22, tessellatedTriWidth);
		glUniform2i(23, width, height);

		glUniform2i(21, gridsize.x, gridsize.y);
		glUniform1f(24, displacementScale);
		glUniform2f(25, size.x, size.y);

		const ImageData &depthTexture = material->getTexture(3)->getImageData(0);
		glViewport(0, 0, depthTexture.width, depthTexture.height);

		glUniformMatrix4fv(12, 1, GL_FALSE, glm::value_ptr(sunMVP));
		glDrawArraysInstanced(GL_PATCHES, 0, 4, patchCount);

		glViewport(0, 0, width, height);
	}
}

void Terrain::keyCallback(const int key, const int scanCode, const int action, const int mods) {
	if (key == GLFW_KEY_O) {
		displacementScale -= displacementScale * 0.03f;
	}
	else if (key == GLFW_KEY_P) {
		displacementScale += displacementScale * 0.03f;
	}

	else if (key == GLFW_KEY_K) {
		tessellatedTriWidth = glm::max(tessellatedTriWidth - 1.0f, 0.0f);
	}
	else if (key == GLFW_KEY_L) {
		tessellatedTriWidth += 1.0f;
	}

	else if (key == GLFW_KEY_G && action == GLFW_PRESS) {
		drawWireframe = !drawWireframe;
	}
	else if (key == GLFW_KEY_F && action == GLFW_PRESS) {
		enableFog = !enableFog;
	}

	else if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
		Material *m = AssetManager::getInstance().getAsset<Material>("terrainMaterial1");
		setMaterial(m);
	}
	else if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
		Material *m = AssetManager::getInstance().getAsset<Material>("terrainMaterial2");
		setMaterial(m);
	}
	else if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
		Material *m = AssetManager::getInstance().getAsset<Material>("terrainMaterial3");
		setMaterial(m);
	}

	else if (key == GLFW_KEY_0 && action == GLFW_PRESS) {
		heightBasedTexturing = !heightBasedTexturing;
	}
}
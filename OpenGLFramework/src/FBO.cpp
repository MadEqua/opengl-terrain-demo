#include "FBO.h"

#include <iostream>

FBO::FBO() : colorAtachmentCount(0) {
	create();
}

FBO::~FBO() {
	destroy();
}

void FBO::create() {
	glCreateFramebuffers(1, &name);
}

void FBO::destroy() {
	glDeleteFramebuffers(1, &name);
}

void FBO::bind() const {
	glBindFramebuffer(GL_FRAMEBUFFER, name);
}

void FBO::bindDefaultFramebuffer() const {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBO::pushToGPU(bool deleteAfterPush) {
	GLenum *drawBuffers = new GLenum[colorAtachmentCount];
	for (int i = 0; i < colorAtachmentCount; ++i) {
		drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
	}
	glNamedFramebufferDrawBuffers(name, colorAtachmentCount, drawBuffers);
	delete[] drawBuffers;


	/*GLenum status = glCheckNamedFramebufferStatus(name, GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Ops?!\n\n";
	}*/
}

void FBO::attachTextureToColorBuffer(int colorAttachmentNumber, const Texture &texture) {
	glNamedFramebufferTexture(name, GL_COLOR_ATTACHMENT0 + colorAttachmentNumber, texture.getName(), 0);
	colorAtachmentCount++;
}

void FBO::attachTextureToDepthBuffer(const Texture &texture) {
	glNamedFramebufferTexture(name, GL_DEPTH_ATTACHMENT, texture.getName(), 0);
}

void FBO::attachTextureToStencilBuffer(const Texture &texture) {
	glNamedFramebufferTexture(name, GL_STENCIL_ATTACHMENT, texture.getName(), 0);
}

void FBO::attachTextureToDepthAndStencilBuffer(const Texture &texture) {
	glNamedFramebufferTexture(name, GL_DEPTH_STENCIL_ATTACHMENT, texture.getName(), 0);
}
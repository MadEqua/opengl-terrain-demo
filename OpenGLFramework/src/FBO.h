#pragma once

#include "GLObject.h"
#include "Texture.h"

class FBO : public GLObject
{
public:
	FBO();
	~FBO();

	virtual void create();
	virtual void destroy();

	virtual void bind() const;
	void bindDefaultFramebuffer() const;
	virtual void pushToGPU(bool deleteAfterPush = false);

	void attachTextureToColorBuffer(int colorAttachmentNumber, const Texture &texture);

	void attachTextureToDepthBuffer(const Texture &texture);
	void attachTextureToStencilBuffer(const Texture &texture);
	void attachTextureToDepthAndStencilBuffer(const Texture &texture);

private:
	int colorAtachmentCount;
};


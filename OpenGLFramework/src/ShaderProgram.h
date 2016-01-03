#pragma once

#include "Shader.h"

#include <vector>
#include <string>

#include "Asset.h"

class ShaderProgram : public Asset
{
public:
	ShaderProgram(const std::vector<Shader*> &shaders);
	//ShaderProgram(const std::vector<std::string> &shaderPaths);
	virtual ~ShaderProgram();

	void bind() const;

private:
	GLuint program;

	void checkLinking(const GLuint program) const;
};


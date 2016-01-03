#pragma once

#include <string>
#include <fstream>
#include <iostream>

#include <GL/glew.h>

#include "Asset.h"

enum ShaderType {
	VERTEX, FRAGMENT, TESSELLATION_CONTROL, TESSELLATION_EVALUATION, GEOMETRY
};

class Shader : public Asset
{
public:
	Shader(const std::string &path);
	virtual ~Shader();

	inline GLuint getGlIdentifier() const { return shader; }
	inline ShaderType getShaderType() const { return type; }

private:
	GLuint shader;
	ShaderType type;

	void inferShaderType(const std::string &path);
	GLenum getGlShaderType(ShaderType type) const;
	void readFromFile(const std::string &path, std::string &out) const;
	void checkCompilation(const GLuint shader) const;
};


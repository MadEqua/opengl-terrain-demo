#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(const std::vector<Shader*> &shaders) {
	program = glCreateProgram();

	for (Shader *s : shaders) {
		glAttachShader(program, s->getGlIdentifier());
	}

	glLinkProgram(program);
	checkLinking(program);
}


/*ShaderProgram::ShaderProgram(const std::vector<std::string> &shaderPaths) {
	program = glCreateProgram();

	for (std::string path : shaderPaths) {
		const Shader *s = AssetManager::getInstance().getShader(path);
		glAttachShader(program, s->getGlIdentifier());
	}

	glLinkProgram(program);
	checkLinking(program);
}*/

ShaderProgram::~ShaderProgram() {
	glDeleteProgram(program);
}
void ShaderProgram::bind() const {
	glUseProgram(program);
}

void ShaderProgram::checkLinking(const GLuint program) const {
	std::cout << "\nShader program " << program << " linking status: \n";

	GLint result;
	glGetProgramiv(program, GL_LINK_STATUS, &result);
	if (!result) {
		std::cout << "Failed to link.\n";
	}
	else {
		std::cout << "Linked sucessfully. :)\n";
	}

	GLint bufflen;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufflen);
	if (bufflen > 1) {
		GLchar* log = new char[bufflen + 1];
		glGetProgramInfoLog(program, bufflen, 0, log);
		std::cout << "Linking log:\n";
		std::cout << log << std::endl;
		delete log;
	}
}
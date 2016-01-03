#include "Shader.h"

Shader::Shader(const std::string &path) {
	std::string code;
	readFromFile(path, code);

	if (!code.empty()) {
		inferShaderType(path);
		shader = glCreateShader(getGlShaderType(type));
		const char *c_str = code.c_str();
		glShaderSource(shader, 1, &c_str, NULL);
		glCompileShader(shader);
		checkCompilation(shader);
	}
	else {
		std::cout << "No shader code found on file " << path << std::endl;
	}
}

Shader::~Shader() {
	glDeleteShader(shader);
}

void Shader::readFromFile(const std::string &path, std::string &out) const {
	std::ifstream file;
	file.open(path);

	std::string str;
	if (file.is_open()) {
		while (std::getline(file, str)) {
			out += str + '\n';
		}
		file.close();
	}
	else {
		std::cout << "Cannot open shader file: " << path << std::endl;
	}
}

void Shader::checkCompilation(const GLuint shader) const {
	std::cout << "\nShader " << shader << " compilation status: \n";

	GLint result;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (!result) {
		std::cout << "Failed to compile.\n";
	}
	else {
		std::cout << "Compiled sucessfully. :)\n";
	}

	GLint bufflen;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &bufflen);
	if (bufflen > 1) {
		GLchar* log = new char[bufflen + 1];
		glGetShaderInfoLog(shader, bufflen, 0, log);
		std::cout << "Compilation log:\n";
		std::cout << log << std::endl;
		delete log;
	}
}

void Shader::inferShaderType(const std::string &path) {
	if (path.find(".ver") != std::string::npos || 
		path.find(".vert") != std::string::npos ||
		path.find(".v") != std::string::npos ||
		path.find(".vs") != std::string::npos) {
		type = VERTEX;
	}
	else if (path.find(".frag") != std::string::npos || 
		path.find(".fra") != std::string::npos ||
		path.find(".f") != std::string::npos ||
		path.find(".fs") != std::string::npos) {
		type = FRAGMENT;
	}
	else if (path.find(".te") != std::string::npos ||
		path.find(".tes") != std::string::npos) {
		type = TESSELLATION_EVALUATION;
	}
	else if (path.find(".tc") != std::string::npos ||
		path.find(".tcs") != std::string::npos) {
		type = TESSELLATION_CONTROL;
	}
	else if (path.find(".geo") != std::string::npos ||
		path.find(".geom") != std::string::npos || 
		path.find(".gs") != std::string::npos) {
		type = GEOMETRY;
	}
}

GLenum Shader::getGlShaderType(ShaderType type) const {
	switch (type)
	{
	case VERTEX:
		return GL_VERTEX_SHADER;
	case FRAGMENT:
		return GL_FRAGMENT_SHADER;
	case TESSELLATION_CONTROL:
		return GL_TESS_CONTROL_SHADER;
	case TESSELLATION_EVALUATION:
		return GL_TESS_EVALUATION_SHADER;
	case GEOMETRY:
		return GL_GEOMETRY_SHADER;

	default:
		return -1;
	}
}
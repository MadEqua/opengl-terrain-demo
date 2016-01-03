#include "VAO.h"

#include "Utils.h"


VAO::VAO() {
	create();
}

VAO::~VAO() {
	destroy();
}

void VAO::create() {
	glCreateVertexArrays(1, &name);
}

void VAO::destroy() {
	glDeleteVertexArrays(1, &name);
}

void VAO::bind() const {
	glBindVertexArray(name);
}

void VAO::enableVertexAttribute(GLuint attributeIndex, GLuint vbo, GLintptr VBOoffset, GLsizei VBOstride) {
	//Associate vertex attributes to buffer bindings
	//glVertexAttribBinding()
	//attributeIndex to the binding id with the same name
	glVertexArrayAttribBinding(name, attributeIndex, attributeIndex);

	//associate VAO buffer bindings to the correct VBOs and with correct offsets
	//PRE 4.5 -> glBindVertexBuffer()
	glVertexArrayVertexBuffer(name, attributeIndex, vbo, VBOoffset, VBOstride);

	//PRE 4.5 -> glEnableVertexAttribArray()
	glEnableVertexArrayAttrib(name, attributeIndex);
}

void VAO::setVertexAttributeFormat(GLuint attributeIndex, GLint size, GLenum type, GLboolean normalized, GLuint relativeOffset) {
	//specify the format of the attributes 
	//PRE 4.5 -> glVertexAttribFormat()
	glVertexArrayAttribFormat(name, attributeIndex, size, type, normalized, relativeOffset);
}
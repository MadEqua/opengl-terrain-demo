#pragma once

#include <GL/glew.h>
#include "GLBuffer.h"

#include <glm/glm.hpp>

#include <cstdint>
#include <vector>

#include "VAO.h"

struct VertexAttributeDescription {
	unsigned int numberOfElements;
	unsigned int elementSizeInBytes;
	GLenum type;
	GLboolean normalized;
};

struct VBODescription {
	std::vector<VertexAttributeDescription> attributes;
};


class VBO : public GLBuffer
{
public:
	VBO(unsigned int numberOfVertices);
	~VBO();
	
	int addAttribute(unsigned int numberOfElements, unsigned int elementSizeInBytes, GLenum type = GL_FLOAT, GLboolean normalized = GL_FALSE);

	void setAttributeData(int attributeId, const void *data); //all the attribute data
	void setVertexAttributeData(int attributeId, int vertexIndex, const void *data); //atribute data for 1 vertex

	virtual void bind() const;
	virtual void pushToGPU(bool deleteAfterPush = true);

	inline const VAO& getVAO() const { return vao; }
	inline unsigned int getNumberOfVertices() const { return numberOfVertices; }

private:
	VBODescription description;
	std::vector<uint8_t*> attributeData; //generic data for each attribute

	unsigned int numberOfVertices;

	VAO vao; //vao that describes this VBO's content

	void addAtributeToVAO(int attributeId);
	
	void deleteLocalData();
};
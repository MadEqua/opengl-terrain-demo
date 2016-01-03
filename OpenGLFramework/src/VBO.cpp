#include "VBO.h"

#include "Utils.h"
#include <iostream>


VBO::VBO(unsigned int numberOfVertices) : numberOfVertices(numberOfVertices) {
	addAttribute(3, sizeof(float)); //position is the attribute 0 by default
}

VBO::~VBO() {
	deleteLocalData();
}

void VBO::deleteLocalData() {
	for (unsigned int i = 0; i < description.attributes.size(); ++i) {
		if (attributeData[i] != 0) {
			delete[] attributeData[i];
			attributeData[i] = 0;
		}
	}
}

void VBO::bind() const {
	glBindBuffer(GL_ARRAY_BUFFER, name);
}

void VBO::pushToGPU(bool deleteAfterPush) {
	for (VertexAttributeDescription desc : description.attributes) {
		sizeInBytes += numberOfVertices * desc.numberOfElements * desc.elementSizeInBytes;
	}

	//Create storage in buffer and send null data
	//4.4 -> glBufferStorage()
	//PRE 4.4 -> glBufferData() (same, but bufffers are mutable, can resize or dealocate)	
	glNamedBufferStorage(name, sizeInBytes, 0, GL_DYNAMIC_STORAGE_BIT);

	int offset = 0;
	for (unsigned int i = 0; i < description.attributes.size(); ++i) {
		VertexAttributeDescription &desc = description.attributes[i];
		GLsizeiptr size = numberOfVertices * desc.numberOfElements * desc.elementSizeInBytes;
		glNamedBufferSubData(name, offset, size, attributeData[i]);
		offset += size;
	}

	if (deleteAfterPush) {
		deleteLocalData();
	}
}

int VBO::addAttribute(unsigned int numberOfElements, unsigned int elementSizeInBytes, GLenum type, GLboolean normalized) {
	VertexAttributeDescription desc;
	desc.numberOfElements = numberOfElements;
	desc.elementSizeInBytes = elementSizeInBytes;
	desc.type = type;
	desc.normalized = normalized;
	description.attributes.push_back(desc);

	int attrIndex = description.attributes.size() - 1;

	uint8_t *data = new uint8_t[numberOfVertices * numberOfElements * elementSizeInBytes];
	attributeData.push_back(data);

	addAtributeToVAO(attrIndex);

	return attrIndex;
}

void VBO::addAtributeToVAO(int attributeId) {
	int offset = 0;
	for (int i = 0; i < attributeId; ++i) {
		offset += description.attributes[i].elementSizeInBytes *
			description.attributes[i].numberOfElements *
			numberOfVertices;
	}

	VertexAttributeDescription &desc = description.attributes[attributeId];
	int stride = desc.elementSizeInBytes * desc.numberOfElements;

	vao.enableVertexAttribute(attributeId, name, offset, stride);
	vao.setVertexAttributeFormat(attributeId, desc.numberOfElements, desc.type, desc.normalized, 0);
}

void VBO::setAttributeData(int attributeId, const void *data) {
	VertexAttributeDescription &desc = description.attributes[attributeId];

	memcpy(attributeData[attributeId],
		data,
		numberOfVertices * desc.numberOfElements * desc.elementSizeInBytes);
}

void VBO::setVertexAttributeData(int attributeId, int vertexIndex, const void *data) {
	VertexAttributeDescription &desc = description.attributes[attributeId];

	uint8_t *offset = attributeData[attributeId] +
		(vertexIndex * desc.elementSizeInBytes * desc.numberOfElements);
	
	memcpy(offset,
		data,
		desc.elementSizeInBytes * desc.numberOfElements);
}
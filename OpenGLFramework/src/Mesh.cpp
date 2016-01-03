#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h> 
#include <iostream>

Mesh::Mesh() : computeTangents(false) {
}	

Mesh::~Mesh() {
	for (VBO *vbo : vbos)
		delete vbo;

	for (IBO *ibo : ibos)
		delete ibo;
}

void Mesh::initFromFile(const std::string &path) {
	Assimp::Importer importer;

	/*aiString list;
	importer.GetExtensionList(list);
	std::cout << list.C_Str() << std::endl;*/

	unsigned int flags = aiProcess_GenSmoothNormals | aiProcess_PreTransformVertices | 
						aiProcess_Triangulate | aiProcess_JoinIdenticalVertices;
	if (computeTangents)
		flags |= aiProcess_CalcTangentSpace;

	const aiScene *scene = importer.ReadFile(path, flags);

	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "Failed to load mesh from file: " << path << ": " << importer.GetErrorString() << std::endl;
		return;
	}
	else {
		initFromAssimpScene(scene);
	}
}

void Mesh::initFromAssimpScene(const aiScene *scene) {
	vbos.resize(scene->mNumMeshes);
	ibos.resize(scene->mNumMeshes);

	for (unsigned int meshI = 0; meshI < scene->mNumMeshes; ++meshI) {
		aiMesh *mesh = scene->mMeshes[meshI];
		unsigned int nVertices = mesh->mNumVertices;
		unsigned int nIndices = mesh->mNumFaces * mesh->mFaces->mNumIndices;

		IBO *ibo = new IBO(nIndices);
		VBO *vbo = new VBO(nVertices);

		vbo->setAttributeData(0, (void*)mesh->mVertices);

		int normalAttr = vbo->addAttribute(3, sizeof(float));
		vbo->setAttributeData(normalAttr, (void*)mesh->mNormals);
		
		if (computeTangents) {
			int tangentAttr = vbo->addAttribute(3, sizeof(float));
			vbo->setAttributeData(tangentAttr, (void*)mesh->mTangents);
		}

		if (mesh->HasTextureCoords(0)) {
			int texCoordAttr = vbo->addAttribute(2, sizeof(float));

			for (unsigned int i = 0; i < nVertices; ++i) {
				vbo->setVertexAttributeData(texCoordAttr, i, (void*)&mesh->mTextureCoords[0][i]);
			}
		}
		if (mesh->HasVertexColors(0)) {
			int colorAttr = vbo->addAttribute(3, sizeof(float));
			vbo->setAttributeData(colorAttr, (void*)mesh->mColors[0]);
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
			for (unsigned int j = 0; j < mesh->mFaces->mNumIndices; ++j) {
				ibo->addIndex(mesh->mFaces[i].mIndices[j]);
			}
		}

		vbo->pushToGPU();
		ibo->pushToGPU();

		vbos[meshI] = vbo;
		ibos[meshI] = ibo;
	}
}

void Mesh::initFromRawData(const int numberOfVertices, const int numberOfIndices) {
	vbos.push_back(new VBO(numberOfVertices));

	if (numberOfIndices > 0)
		ibos.push_back(new IBO(numberOfIndices));
}

void Mesh::setAttributeData(int attributeId, const void *data) {
	vbos[0]->setAttributeData(attributeId, data);
}

void Mesh::setVertexAttributeData(int attributeId, int vertexIndex, const void *data) {
	vbos[0]->setVertexAttributeData(attributeId, vertexIndex, data);
}

int Mesh::addAttribute(unsigned int numberOfElements, unsigned int elementSizeInBytes, GLenum type, GLboolean normalized) {
	return vbos[0]->addAttribute(numberOfElements, elementSizeInBytes, type, normalized);
}

void Mesh::setIndices(GLuint *indices) {
	ibos[0]->setIndices(indices);
}

void Mesh::addIndex(GLuint index) {
	ibos[0]->addIndex(index);
}

void Mesh::endFromRawData() {
	vbos[0]->pushToGPU();

	if (!ibos.empty())
		ibos[0]->pushToGPU();
}

void Mesh::renderIndexed(GLenum mode) const {
	for (unsigned int i = 0; i < vbos.size(); ++i) {
		vbos[i]->getVAO().bind();
		ibos[i]->bind();
		glDrawElements(mode, ibos[i]->getNumberOfIndices(), GL_UNSIGNED_INT, 0);
	}
}

void Mesh::render(GLenum mode) const {
	for (unsigned int i = 0; i < vbos.size(); ++i) {
		vbos[i]->getVAO().bind();
		glDrawArrays(mode, 0, vbos[i]->getNumberOfVertices());
	}
}

void Mesh::renderInstanced(GLenum mode) const {
	for (unsigned int i = 0; i < vbos.size(); ++i) {
		vbos[i]->getVAO().bind();
		ibos[i]->bind();
		glDrawElementsInstanced(mode, ibos[i]->getNumberOfIndices(), GL_UNSIGNED_INT, 0, instanceCount);
	}
}

void Mesh::setInstancedRenderProperties(int instanceCount, int index, int divisor) { 
	this->instanceCount = instanceCount; 
	for (unsigned int i = 0; i < vbos.size(); ++i) {
		vbos[i]->getVAO().bind();
		glVertexAttribDivisor(index, divisor);
	}
}
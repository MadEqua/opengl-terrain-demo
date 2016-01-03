#pragma once

#include <GL/glew.h>
#include <string>
#include <vector>

#include <assimp/scene.h>

#include "Asset.h"

#include "VBO.h"
#include "IBO.h"

class Mesh : public Asset
{
public:
	Mesh();
	virtual ~Mesh();

	void renderIndexed(GLenum mode = GL_TRIANGLES) const;
	void render(GLenum mode = GL_TRIANGLES) const;
	void renderInstanced(GLenum mode = GL_TRIANGLES) const;

	void setInstancedRenderProperties(int instanceCount, int index, int divisor);

	void initFromFile(const std::string &path);
	void initFromRawData(const int numberOfVertices, const int numberOfIndices);

	void setAttributeData(int attributeId, const void *data); //all the attribute data
	void setVertexAttributeData(int attributeId, int vertexIndex, const void *data); //atribute data for 1 vertex
	int  addAttribute(unsigned int numberOfElements, unsigned int elementSizeInBytes, GLenum type = GL_FLOAT, GLboolean normalized = GL_FALSE);

	void endFromRawData();

	void setIndices(GLuint *indices);
	void addIndex(GLuint index);

	inline void setComputeTangents(bool v) { computeTangents = v; }
	
private:
	void initFromAssimpScene(const aiScene *scene);

	//One per sub-mesh
	std::vector<VBO*> vbos;
	std::vector<IBO*> ibos;

	//when rendering instanced
	int instanceCount;

	bool computeTangents;
};

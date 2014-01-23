#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include <glm/glm.hpp>
#include "utils.h"

class Model {
public:
	~Model();
	static Model *creator(unsigned int program, std::vector<glm::vec3> &v, std::vector<unsigned int> &i);
	unsigned int numIndices;
	unsigned int normalsCount;
	unsigned int numVerts;
	unsigned int numFaces;
	unsigned int texCount;
	unsigned int vertAttribLoc;
	unsigned int normAttribLoc;
	unsigned int texAttribLoc;
	std::vector<glm::vec3> vertexArray;
	float *normalArray;
	float *texCoords;
	std::vector<unsigned int> indexArray;
	unsigned int *normalsIndex;
	unsigned int vao;
	static void generateNormals(Model *m);
protected:
	Model();
	

private:

};

#endif //__MODEL_H__

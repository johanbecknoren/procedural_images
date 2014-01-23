#include "model.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "externals/GL/GL/glew.h"
#include "GL/glfw.h"
#define PI 3.14159265359f

Model::Model() {

}

Model::~Model() {
	//delete[] normalArray;
	//delete[] texCoords;
	//delete[] normalsIndex;
}

Model *Model::creator(GLuint program, std::vector<glm::vec3> &verts, std::vector<unsigned int> &indices) {
	Model *model = new Model();
	model->numIndices = (unsigned int) indices.size();
	model->numVerts = (unsigned int) verts.size();
	model->numFaces = (unsigned int) indices.size() / 3;
	model->vertAttribLoc = 0;
	model->normAttribLoc = 1;
	model->texAttribLoc = 2;

	model->indexArray = indices;
	//std::copy(&model->indexArray, &model->indexArray + model->numIndices, verts.begin());
	std::vector<unsigned int>::iterator iit = model->indexArray.begin();
	model->vertexArray = verts;
	std::vector<glm::vec3>::iterator vit = model->vertexArray.begin();

	glGenVertexArrays(1, &model->vao);
	glBindVertexArray(model->vao);

	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		sizeof(unsigned int) * model->numFaces * 3,
		&(*iit), GL_STATIC_DRAW);
	char *vertVarName = "in_Position";
	char *normVarName = "in_Normal";
	char *texCoordVarName = "in_texCoord";
		
	glBindAttribLocation(program, model->vertAttribLoc, vertVarName);
	glBindAttribLocation(program, model->normAttribLoc, normVarName);
	glBindAttribLocation(program, model->texAttribLoc, texCoordVarName);

	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(float) * 3 * model->numVerts,
		glm::value_ptr(verts[0]), GL_STATIC_DRAW);
	generateNormals(model);
	return model;
}

void Model::generateNormals(Model* mesh)
{
	// If model has vertices but no vertexnormals, generate normals
	if (mesh->vertexArray.size() > 0 && !mesh->normalArray)
	{
		unsigned int face;
		unsigned int normalIndex;

		mesh->normalArray = new GLfloat[3 * mesh->numVerts];
		memset(mesh->normalArray, 0, 3 * sizeof(GLfloat) * mesh->numVerts);

		mesh->normalsCount = mesh->numVerts;

		mesh->normalsIndex = new GLuint[mesh->numIndices];
		memcpy(mesh->normalsIndex, &mesh->indexArray[0],
			sizeof(GLuint) * mesh->numIndices);
		GLfloat *first = glm::value_ptr(mesh->vertexArray[0]);
		for (face = 0; face * 3 < mesh->numIndices; face++)
		{
			int i0 = mesh->indexArray[face * 3 + 0];
			int i1 = mesh->indexArray[face * 3 + 1];
			int i2 = mesh->indexArray[face * 3 + 2];
				
			GLfloat* vertex0 = first + i0 * 3;
			GLfloat* vertex1 = first + i1 * 3;
			GLfloat* vertex2 = first + i2 * 3;

			float v0x = vertex1[0] - vertex0[0];
			float v0y = vertex1[1] - vertex0[1];
			float v0z = vertex1[2] - vertex0[2];

			float v1x = vertex2[0] - vertex0[0];
			float v1y = vertex2[1] - vertex0[1];
			float v1z = vertex2[2] - vertex0[2];

			float v2x = vertex2[0] - vertex1[0];
			float v2y = vertex2[1] - vertex1[1];
			float v2z = vertex2[2] - vertex1[2];

			float sqrLen0 = v0x * v0x + v0y * v0y + v0z * v0z;
			float sqrLen1 = v1x * v1x + v1y * v1y + v1z * v1z;
			float sqrLen2 = v2x * v2x + v2y * v2y + v2z * v2z;

			float len0 = (sqrLen0 >= 1e-6f) ? sqrt(sqrLen0) : 1e-3f;
			float len1 = (sqrLen1 >= 1e-6f) ? sqrt(sqrLen1) : 1e-3f;
			float len2 = (sqrLen2 >= 1e-6f) ? sqrt(sqrLen2) : 1e-3f;

			float influence0 = (v0x * v1x + v0y * v1y + v0z * v1z) / (len0 * len1);
			float influence1 = -(v0x * v2x + v0y * v2y + v0z * v2z) / (len0 * len2);
			float influence2 = (v1x * v2x + v1y * v2y + v1z * v2z) / (len1 * len2);

			float angle0 = (influence0 >= 1.f) ? 0 : 
				(influence0 <= -1.f) ? PI : acos(influence0);
			float angle1 = (influence1 >= 1.f) ? 0 : 
				(influence1 <= -1.f) ? PI : acos(influence1);
			float angle2 = (influence2 >= 1.f) ? 0 : 
				(influence2 <= -1.f) ? PI : acos(influence2);

			float normalX = v1z * v0y - v1y * v0z;
			float normalY = v1x * v0z - v1z * v0x;
			float normalZ = v1y * v0x - v1x * v0y;

			GLfloat* normal0 = &mesh->normalArray[i0 * 3];
			GLfloat* normal1 = &mesh->normalArray[i1 * 3];
			GLfloat* normal2 = &mesh->normalArray[i2 * 3];

			normal0[0] += normalX * angle0;
			normal0[1] += normalY * angle0;
			normal0[2] += normalZ * angle0;

			normal1[0] += normalX * angle1;
			normal1[1] += normalY * angle1;
			normal1[2] += normalZ * angle1;

			normal2[0] += normalX * angle2;
			normal2[1] += normalY * angle2;
			normal2[2] += normalZ * angle2;
		}

		for (normalIndex = 0; normalIndex < mesh->normalsCount; normalIndex++)
		{
			GLfloat* normal = &mesh->normalArray[normalIndex * 3];
			float length = sqrt(normal[0] * normal[0] + normal[1] * normal[1]
			+ normal[2] * normal[2]);
			float reciprocalLength = 1.f;

			if (length > 0.01f)
				reciprocalLength = 1.f / length;

			normal[0] *= reciprocalLength;
			normal[1] *= reciprocalLength;
			normal[2] *= reciprocalLength;
		}
	}
}

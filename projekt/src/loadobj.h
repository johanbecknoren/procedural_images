#ifndef loadobj_h
#define loadobj_h

#ifdef __cplusplus
//extern "C" {
#endif

#include <string>
#include "utils.h"

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
#else
	//#include <Windows.h>
	/*#include <gl/glew.h>
	#include <GL/freeglut.h>*/
#endif

typedef struct
{
  float* vertexArray;
  float* normalArray;
  float* texCoordArray;
  float* colorArray; // Rarely used
  unsigned int* indexArray;
  int numVertices;
  int numIndices;
  
  // Space for saving VBO and VAO IDs
  unsigned int vao; // VAO
  unsigned int vb, ib, nb, tb; // VBOs
} Model;

Model* LoadModel(char* name);

// NEW:

void DrawModel(Model *m);
void DrawQuad();
void DrawCube();
void DrawWireframeModel(Model *m);

Model* LoadModelPlus(char* name,
			unsigned int program,
			char* vertexVariableName,
			char* normalVariableName,
			char* texCoordVariableName);
void EnableModelForShader(Model *m, unsigned int program, // NOT TESTED
			char* vertexVariableName,
			char* normalVariableName,
			char* texCoordVariableName);
Model* LoadDataToModel(
			float *vertices,
			float *normals,
			float *texCoords,
			float *colors,
			unsigned int *indices,
			int numVert,
			int numInd,
			
			unsigned int program,
			char* vertexVariableName,
			char* normalVariableName,
			char* texCoordVariableName);
void BuildModelVAO(Model *m,
			unsigned int program,
			char* vertexVariableName,
			char* normalVariableName,
			char* texCoordVariableName);

void CenterModel(Model *m);

std::string fixPath(std::string localPath);

#ifdef __cplusplus
//}
#endif

#endif

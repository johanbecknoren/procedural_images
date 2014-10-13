#ifndef TERRAIN_H
#define TERRAIN_H

#include "utils.h"
#include "fbo.h"
#include "loadobj.h"
#include "shadermanager.h"
#include "cam.h"

class Terrain {
public:
	Terrain();
	~Terrain() {
		delete[] _vertices;
		delete[] _vertexnormals;
	};

	void init();
	void render(const glm::mat4 &MV, const glm::mat4 &proj);
	void renderPatches(const Camera& cam);
	void reloadShaders();

	bool getWireframeRender() const { return drawWireframe; }
	void setWireRender(const bool &b) { drawWireframe = b; }

	bool generateGrid();

private:
	void loadShaders();
	void printError(const char *functionName);
	unsigned int getVertexCount() const { return kGridHeight*kGridHeight*3; }
	unsigned int getIndexCount() const { return kGridHeight*kGridWidth + (kGridWidth-1)*(kGridHeight-2); }

private:
	Fbo* fbo1;
	Fbo* fbo2;

	Model* box;
	Model* quad;

	GLfloat* _vertices;
	GLuint* _vertexIndices;
	GLfloat* _vertexnormals;

	GLuint _vao,_vb,_ib;

	ShaderManager shaderManager;

	bool drawWireframe;
};
#endif TERRAIN_H
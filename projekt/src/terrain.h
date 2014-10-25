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
		delete[] _vertexNormals;
	};

	void init();
	void render(const glm::mat4 &MV, const glm::mat4 &proj, const glm::vec3 &campos);
	void renderPatches(const Camera& cam);
	void drawTerrain();
	void reloadShaders();
	void updateNumOctaves(const int& n) { if((_numOctaves + n) > 0) _numOctaves += n; };
	void updateWaterLevel(const float& n) { if((_waterLevel + n) > 0.f && (_waterLevel + n) <= 1.0f) _waterLevel += n; };

	bool getWireframeRender() const { return _drawWireframe; }
	void toggleWireRender() { _drawWireframe = !_drawWireframe; }

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
	GLfloat* _vertexNormals;

	GLuint _vao,_vb,_ib,_nb;

	int _numOctaves;
	float _waterLevel; // [0,1]

	ShaderManager shaderManager;

	bool _drawWireframe;
};
#endif TERRAIN_H
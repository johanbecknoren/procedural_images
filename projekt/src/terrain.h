#ifndef TERRAIN_H
#define TERRAIN_H

#include "utils.h"
#include "fbo.h"
#include "loadobj.h"
#include "shadermanager.h"

class Terrain {
public:
	Terrain();
	~Terrain() {};

	void init();
	void render(const glm::mat4 &MV, const glm::mat4 &proj);
	void loadShaders();
	void printError(const char *functionName);

private:
	Fbo* fbo1;
	Fbo* fbo2;

	Model* box;
	Model* quad;

	ShaderManager shaderManager;
};
#endif TERRAIN_H
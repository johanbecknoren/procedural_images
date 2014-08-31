#include "gl/glew.h"
#include "terrain.h"

#include <glm/gtc/type_ptr.hpp>

void Terrain::printError(const char *functionName)
{
	GLenum error;
	while (( error = glGetError() ) != GL_NO_ERROR)
	{
		fprintf (stderr, "GL error 0x%X detected in %s\n", error, functionName);
	}
}

void Terrain::loadShaders() {
	shaderManager.loadShaders("mainshader.vert", "mainshader.frag", ShaderManager::shaderId::MAIN);
	//shaderManager.loadShadersTG("tessellation.vert", "tessellation.frag", "tessellation.tcs", "tessellation.tes", "", ShaderManager::shaderId::MAIN);
	shaderManager.loadShaders("textureToScreen.vert", "textureToScreen.frag", ShaderManager::TEX2SCREEN);
}

void initGL() {
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	GLint maxPatchVerts = 0;
	glGetIntegerv(GL_MAX_PATCH_VERTICES, &maxPatchVerts);
	std::cout << "Max supported patch vertices: "<<maxPatchVerts<<std::endl;
}

Terrain::Terrain() {

}

void Terrain::reloadShaders() {
	loadShaders();
}

void Terrain::init() {
	initGL();
	printError("Init GL");

	fbo1 = new Fbo(kWidth, kHeight, 0);
	printError("Error init fbo1");
	fbo2 = new Fbo(kWidth, kHeight, 0);
	printError("Error init fbo2");

	shaderManager = ShaderManager();

	loadShaders();

	printError("Load shaders");

	box = LoadModelPlus(const_cast<char*>(fixPath("cube.obj").c_str()), 
		shaderManager.getId(ShaderManager::MAIN), 
		"in_Position", "in_Normal", "in_texCoord");
	printError("Load models 1");

	quad = LoadModelPlus(const_cast<char*>(fixPath("quad.obj").c_str()),
		shaderManager.getId(ShaderManager::TEX2SCREEN),
		"in_Position", "in_Normal", "in_texCoord");
	printError("Load models 2");
}

void Terrain::render(const glm::mat4 &MV, const glm::mat4 &proj) {
	glm::mat4 mvp = proj * MV;
	Fbo::useFbo(fbo1, 0L, 0L);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glUseProgram(shaderManager.getId(ShaderManager::shaderId::MAIN));
	glUniformMatrix4fv(
		glGetUniformLocation(shaderManager.getId(ShaderManager::shaderId::MAIN), "camTrans"), 
		1, GL_FALSE, glm::value_ptr(mvp) );
	if(!drawWireframe)
		DrawModel(box);
	else
		DrawWireframeModel(box);

	printError("Draw box");
	glFlush();

	Fbo::useFbo(0L,fbo1, 0L);
	//Fbo::useFbo(0L,0L,0L);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.5f, 0.5f, 0.5f, 0.f);
	glUseProgram(shaderManager.getId(ShaderManager::shaderId::TEX2SCREEN));
	glUniform1i(
		glGetUniformLocation(shaderManager.getId(ShaderManager::shaderId::TEX2SCREEN), "texUnit"), 
		0);
	DrawModel(quad);
	printError("Draw viewport quad");
	glFlush();
}

void Terrain::renderPatches(const Camera& cam) {
	// --- Create quad here, eval. and try tessellation shader here.

	// Setup stuff
	GLint maxPatchVerts = 0;
	glGetIntegerv(GL_MAX_PATCH_VERTICES, &maxPatchVerts);
	std::cout << "Max supported patch vertices: "<<maxPatchVerts<<std::endl;
}
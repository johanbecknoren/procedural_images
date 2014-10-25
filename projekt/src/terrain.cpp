#include "gl/glew.h"
#include "terrain.h"

#include <glm/gtc/type_ptr.hpp>

#define RENDER_GRID

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
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_SRC_ALPHA, GL_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	GLint maxPatchVerts = 0;
	glGetIntegerv(GL_MAX_PATCH_VERTICES, &maxPatchVerts);
	std::cout << "Max supported patch vertices: "<<maxPatchVerts<<std::endl;
}

Terrain::Terrain() {
	_numOctaves = 6;
	_waterLevel = 0.4f;
	_drawWireframe = false;
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

#ifndef RENDER_GRID
	box = LoadModelPlus(const_cast<char*>(fixPath("cube.obj").c_str()), 
		shaderManager.getId(ShaderManager::MAIN), 
		"in_Position", "in_Normal", "in_texCoord");
	printError("Load models 1");

	quad = LoadModelPlus(const_cast<char*>(fixPath("quad.obj").c_str()),
		shaderManager.getId(ShaderManager::TEX2SCREEN),
		"in_Position", "in_Normal", "in_texCoord");
	printError("Load models 2");
#else
	generateGrid();
	printError("generateGrid()");
#endif
}

void indexTo2D(const uint32_t i, uint32_t &col, uint32_t &row)
{
	col = i/kGridWidth;
	row = i%kGridHeight;
}

bool Terrain::generateGrid()
{
	unsigned int numVertices = getVertexCount();
	unsigned int numIndices = getIndexCount();
	unsigned int numNormals = getVertexCount();

	// Allocate arrays
	_vertices = new GLfloat[numVertices];
	_vertexIndices = new GLuint[numIndices];
	_vertexNormals = new GLfloat[numNormals];

	unsigned int i = 0;

	for(unsigned int row=0; row<kGridHeight; ++row)
	{
		for(unsigned int col=0; col<kGridWidth; ++col)
		{
			_vertices[i++] = static_cast<GLfloat>(row)*kGridPointSpacing;
			_vertices[i++] = -10.0f;
			_vertices[i++] = static_cast<GLfloat>(col)*kGridPointSpacing;
		}
	}

	i = 0;

	for (unsigned int row=0; row<kGridHeight-1; row++ ) 
	{
        if ((row&1)==0) 
		{ // even rows
            for ( int col=0; col<kGridWidth; col++ ) 
			{
                _vertexIndices[i++] = col + row * kGridWidth;
                _vertexIndices[i++] = col + (row+1) * kGridWidth;
            }
        }
		else 
		{ // odd rows
            for (unsigned int col=kGridWidth-1; col>0; col-- ) 
			{
                _vertexIndices[i++] = col + (row+1) * kGridWidth;
                _vertexIndices[i++] = col - 1 + + row * kGridWidth;
            }
        }
    }

	i = 0;

	for(unsigned int row=0; row<kGridHeight; ++row)
	{
		for(unsigned int col=0; col<kGridWidth; ++col)
		{
			_vertexNormals[i++] = 0.f;
			_vertexNormals[i++] = 1.f;
			_vertexNormals[i++] = 0.f;
		}
	}

	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vb);
	glGenBuffers(1, &_ib);
	glGenBuffers(1, &_nb);

	glBindVertexArray(_vao);
	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, _vb);
	glBufferData(GL_ARRAY_BUFFER, getVertexCount()*sizeof(GLfloat), _vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(
		glGetAttribLocation(shaderManager.getId(ShaderManager::MAIN), "in_Position"), 3, GL_FLOAT, GL_FALSE,0,0);
	glEnableVertexAttribArray(glGetAttribLocation(shaderManager.getId(ShaderManager::MAIN), "in_Position"));

	/*VBO for normal data here if needed*/
	glBindBuffer(GL_ARRAY_BUFFER, _nb);
	glBufferData(GL_ARRAY_BUFFER, getVertexCount()*sizeof(GLfloat), _vertexNormals, GL_STATIC_DRAW);
	glVertexAttribPointer(
		glGetAttribLocation(shaderManager.getId(ShaderManager::MAIN), "in_Normal"), 3, GL_FLOAT, GL_FALSE,0,0);
	glEnableVertexAttribArray(glGetAttribLocation(shaderManager.getId(ShaderManager::MAIN), "in_Normal"));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ib);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, getIndexCount()*sizeof(GLuint), _vertexIndices, GL_STATIC_DRAW);

	printError("Binding buffers");
	return true;
}

void Terrain::drawTerrain()
{
	glBindVertexArray(_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ib);
	
	if(_drawWireframe)
		glDrawElements(GL_LINE_LOOP, getIndexCount(), GL_UNSIGNED_INT, 0L);
	else
		glDrawElements(GL_TRIANGLE_STRIP, getIndexCount(), GL_UNSIGNED_INT, 0L);
		
}

void Terrain::render(const glm::mat4 &MV, const glm::mat4 &proj, const glm::vec3 &campos) {

	glm::mat4 mvp = proj * MV;
	//Fbo::useFbo(fbo1, 0L, 0L); // Draw to a Frame Buffer Object
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glClearColor(0.5f, 0.5f, 0.5f, 0.f);
	glClearColor(0.5f, .5f, 0.5f, 0.f);
	glUseProgram(shaderManager.getId(ShaderManager::shaderId::MAIN));
	glUniformMatrix4fv(
		glGetUniformLocation(shaderManager.getId(ShaderManager::shaderId::MAIN), "mvp"), 
		1, GL_FALSE, glm::value_ptr(mvp) );
	printError("mvp");
	glUniform1ui(glGetUniformLocation(shaderManager.getId(ShaderManager::MAIN), "gridWidth"),kGridWidth);
	glUniform1ui(glGetUniformLocation(shaderManager.getId(ShaderManager::MAIN), "gridHeight"),kGridHeight);
	glUniform1f(glGetUniformLocation(shaderManager.getId(ShaderManager::MAIN), "gridSpacing"),kGridPointSpacing);
	printError("grid dimensions");
	glUniform3fv(glGetUniformLocation(shaderManager.getId(ShaderManager::MAIN), "camPos"), 1, glm::value_ptr(campos));
	printError("camera position");
	glUniform1i(glGetUniformLocation(shaderManager.getId(ShaderManager::MAIN), "numberOfOctaves"),_numOctaves);
	glUniform1f(glGetUniformLocation(shaderManager.getId(ShaderManager::MAIN), "waterLevel"),_waterLevel);
	printError("other");
#ifndef RENDER_GRID	
	if(!drawWireframe)
		DrawModel(box);
	else
		DrawWireframeModel(box);

	printError("Draw box");
	glFlush();

	// Draw full screen quad
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
#else
	
	drawTerrain();
	printError("Terrain render");
	//glFlush();
	glFinish();
#endif
}

void Terrain::renderPatches(const Camera& cam) {
	// --- Create quad here, eval. and try tessellation shader here.

	// Setup stuff
	GLint maxPatchVerts = 0;
	glGetIntegerv(GL_MAX_PATCH_VERTICES, &maxPatchVerts);
	std::cout << "Max supported patch vertices: "<<maxPatchVerts<<std::endl;
}
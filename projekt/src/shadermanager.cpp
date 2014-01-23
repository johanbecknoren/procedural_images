#include "shadermanager.h"

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <sstream>
//#include "glincludes.h"
#include "externals/GL/GL/glew.h"
#include "gl/glfw.h"
#include "utils.h"


ShaderManager::ShaderManager() {
}

char* ShaderManager::readFile(char *file)
{
	FILE *fptr;
	long length;
	char *buf = NULL;

	fptr = fopen(file, "rb"); /* Open file for reading */
	if (!fptr) /* Return NULL on failure */
		return NULL;
	fseek(fptr, 0, SEEK_END); /* Seek to the end of the file */
	length = ftell(fptr); /* Find out how many bytes into the file we are */
	buf = (char*)malloc(length+1); /* Allocate a buffer for the entire length of the file and a null terminator */
	fseek(fptr, 0, SEEK_SET); /* Go back to the beginning of the file */
	fread(buf, length, 1, fptr); /* Read the contents of the file in to the buffer */
	fclose(fptr); /* Close the file */
	buf[length] = 0; /* Null terminator */
	
	return buf; /* Return the buffer */
}

// Infolog: Show result of shader compilation
void ShaderManager::printShaderInfoLog(GLuint obj, const char *fn)
{
	GLint infologLength = 0;
	GLint charsWritten  = 0;
	char *infoLog;

	glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

	if (infologLength > 1)
	{
		printf("[From %s:]\n", fn);
		infoLog = (char *)malloc(infologLength);
		glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n",infoLog);
		free(infoLog);
	}
}

void ShaderManager::printProgramInfoLog(GLuint obj, const char *vfn, const char *ffn, const char *gfn)
{
	GLint infologLength = 0;
	GLint charsWritten  = 0;
	char *infoLog;

	glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

	if (infologLength > 1)
	{
		if (gfn == NULL)
			printf("[From %s+%s:]\n", vfn, ffn);
		else
			printf("[From %s+%s+%s:]\n", vfn, ffn, gfn);
		infoLog = (char *)malloc(infologLength);
		glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n",infoLog);
		free(infoLog);
	}
}

// Compile a shader, return reference to it
GLuint ShaderManager::compileShaders(const char *vs, const char *fs, const char *gs, const char *vfn, const char *ffn, const char *gfn)
{
	GLuint v,f,g,p;
	p = 0;

	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(v, 1, &vs, NULL);
	glShaderSource(f, 1, &fs, NULL);
	glCompileShader(v);
	glCompileShader(f);
	if (gs != NULL)
	{
		g = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(g, 1, &gs, NULL);
		glCompileShader(g);
	}

	p = glCreateProgram();
	glAttachShader(p,v);
	glAttachShader(p,f);
	if (gs != NULL)
		glAttachShader(p,g);
	glLinkProgram(p);
	glUseProgram(p);

	printShaderInfoLog(v, vfn);
	printShaderInfoLog(f, ffn);
	if (gs != NULL)	printShaderInfoLog(g, gfn);

	printProgramInfoLog(p, vfn, ffn, gfn);
	
	return p;
}

GLuint ShaderManager::getId(shaderId id) {
	return shaders[id];
}

bool ShaderManager::loadShaders(std::string vertFileName, std::string fragFileName, shaderId id)
{
	GLuint sid = loadShaderG(fixPath(vertFileName).c_str(), fixPath(fragFileName).c_str(), NULL);
	shaders[id] = sid;
	return true;
}
std::string ShaderManager::fixPath(std::string localPath) {
	std::stringstream fullPath(CMAKE_PROJECT_ROOT_DIR);
	fullPath << CMAKE_PROJECT_ROOT_DIR << "/src/shaders/" << localPath;
	return fullPath.str();
}


bool ShaderManager::loadShadersG(std::string vertFileName, std::string fragFileName, 
								 std::string geomFileName, shaderId id)
{
	
	GLuint sid = loadShaderG(fixPath(vertFileName).c_str(), 
		fixPath(fragFileName).c_str(), fixPath(geomFileName).c_str());
	shaders[id] = sid;
	return true;
}

GLuint ShaderManager::loadShader(const char *vertFileName, const char *fragFileName)
{
	return loadShaderG(vertFileName, fragFileName, NULL);
}

GLuint ShaderManager::loadShaderG(const char *vertFileName, const char *fragFileName, const char *geomFileName)
// With geometry shader support
{
	char *vs, *fs, *gs;
	GLuint p = 0;
	vs = readFile((char *)vertFileName);
	fs = readFile((char *)fragFileName);
    if(geomFileName != NULL)
        gs = readFile((char *)geomFileName);
    else
        gs = NULL;
	if (vs==NULL)
		printf("Failed to read %s from disk.\n", vertFileName);
	if (fs==NULL)
		printf("Failed to read %s from disk.\n", fragFileName);
	if ((gs==NULL) && (geomFileName != NULL))
		printf("Failed to read %s from disk.\n", geomFileName);
	if ((vs!=NULL)&&(fs!=NULL))
		p = compileShaders(vs, fs, gs, vertFileName, fragFileName, geomFileName);
	if (vs != NULL) free(vs);
	if (fs != NULL) free(fs);
	if (gs != NULL) free(gs);
	printf("Shader loaded with id %i \n", p);
	glUseProgram(0);
	return p;
}
// End of ShaderLoader

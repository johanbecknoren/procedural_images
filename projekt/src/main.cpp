#include <iostream>
#include "glew.h"
#include "gl/glfw.h"

#include <glm/gtc/type_ptr.hpp>

#include "utils.h"
#include "cam.h"
#include "fbo.h"
//#include "model.h"
#include "loadobj.h"
#include "shadermanager.h"

Camera cam(kWidth, kHeight);

//GLfloat verts[9] = {0.0f,0.0f,0.0f, 0.0f,1.0f,0.0f, 1.0f,1.0f,0.0f};
//GLfloat normal[3] = {0.0f,0.0f,-1.0f};
//GLfloat colors[9] = {0.0f,0.0f,0.0f, 0.0f,1.0f,0.0f, 1.0f,1.0f,0.0f};

Fbo* fbo1;
Fbo* fbo2;

Model* box;
Model* quad;

ShaderManager shaderManager;

void printError(const char *functionName)
{
	GLenum error;
	while (( error = glGetError() ) != GL_NO_ERROR)
	{
		fprintf (stderr, "GL error 0x%X detected in %s\n", error, functionName);
	}
}

void loadShaders() {
	shaderManager.loadShaders("mainshader.vert", "mainshader.frag", ShaderManager::shaderId::MAIN);
	shaderManager.loadShaders("textureToScreen.vert", "textureToScreen.frag", ShaderManager::TEX2SCREEN);
}

// Callback functions for GLFW window context
static void key_callback(int key, int action ,int mods) {
	std::cout<<"key="<<key<<std::endl;
	if(key == 'O') {
		if(action == GLFW_PRESS) {
			//loadShaders();
		}
	}
	if (key == 'W') {// W
		if(action == GLFW_PRESS) {
			cam.holdingForward = true;
		} else if (action == GLFW_RELEASE) {
			cam.holdingForward = false;
		}
	} else if(key == 'S') {
		if(action == GLFW_PRESS) {
			cam.holdingBackward = true;
		} else if(action == GLFW_RELEASE) {
			cam.holdingBackward = false;
		}
	} else if(key == 'A') {
		if(action == GLFW_PRESS) {
			cam.holdingLeftStrafe = true;
		} else if(action == GLFW_RELEASE) {
			cam.holdingLeftStrafe = false;
		}
	} else if(key == 'D') {
		if(action == GLFW_PRESS) {
			cam.holdingRightStrafe = true;
		} else if(action == GLFW_RELEASE) {
			cam.holdingRightStrafe = false;
		}
	}
}

static void handle_mouse_move(double mouse_x, double mouse_y) {
	//std::cout<<"mouse_x:"<<mouse_x<<",mouse_y:"<<mouse_y<<std::endl;
	cam.handleMouseMove(mouse_x, mouse_y);
}

int main(int argc, char** argv) {

	// Init GL stuff
	glfwInit();

	if(!glfwOpenWindow(kWidth, kHeight, 8,8,8,8, 32, 0, GLFW_WINDOW)) {
		glfwTerminate();
		std::cout<<"Error init GLFW\n";
		return 0;
	}

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();

	if(err != GLEW_OK) {
		std::cout<<"Error initializing GLEW, exiting...\n";
		return 0;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(real(0),real(0),real(0),real(0));
	glfwSwapInterval(0);
	glfwSetKeyCallback((GLFWkeyfun)key_callback);
	//glfwSetMousePosCallback((GLFWmouseposfun)handle_mouse_move);
	real* pixels = new real[3*kWidth*kHeight]();

	for(size_t i=0; i< 3*kWidth*kHeight; i += 3) {
		pixels[i + 0] = real(0.5);
		pixels[i + 1] = real(0.5);
		pixels[i + 2] = real(1.0);
	}

	float currentTime, lastTime = 0.0f;
	float deltaT = 0.01f;

	// TEMP GL-STUFF TO TEST CAMERA CLASS!
	// This stuff maybe in terrain->init() instead.
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

	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	// -- END

	bool quit = false;
	while(!quit) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		currentTime = float(glfwGetTime());
		deltaT = currentTime - lastTime;
		lastTime = currentTime;

		cam.move(deltaT);
		printError("Error moving camera");


		// Render stuff here!
		// E.g terrain.render(cam.getModelView(), cam.getProjection());
		//glDrawPixels(kWidth, kHeight, GL_RGB, GL_FLOAT, pixels);
		
		// TEMP RENDER STUFF
			Fbo::useFbo(fbo1, 0L, 0L);
			glm::mat4 mvp = cam.getProjection() * cam.getModelView();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glClearColor(0.f, 0.f, 0.f, 0.f);
			glUseProgram(shaderManager.getId(ShaderManager::shaderId::MAIN));
			glUniformMatrix4fv(
				glGetUniformLocation(shaderManager.getId(ShaderManager::shaderId::MAIN), "camTrans"), 
				1, GL_FALSE, glm::value_ptr(mvp) );
			DrawModel(box);
			printError("Draw box");
			glFlush();

			Fbo::useFbo(0L,fbo1, 0L);
			//Fbo::useFbo(0L,0L,0L);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glClearColor(0.f, 0.f, 0.f, 0.f);
			glUseProgram(shaderManager.getId(ShaderManager::shaderId::TEX2SCREEN));
			glUniform1i(
				glGetUniformLocation(shaderManager.getId(ShaderManager::shaderId::TEX2SCREEN), "texUnit"), 
				0);
			DrawModel(quad);
			printError("Draw viewport quad");
			glFlush();

			/*Fbo::useFbo(0L,0L,0L);
			glUseProgram(0);*/
		// -- END

		// Swap buffers
		//glfwPollEvents();
		glfwSwapBuffers();
		printError("Swap buffers");
		if(glfwGetKey('Q') || !glfwGetWindowParam(GLFW_OPENED) ) {
			quit = true;
		}
	}
	glfwTerminate();
	return 0;
}
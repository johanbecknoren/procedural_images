#include <iostream>
#include "glew.h"
#include "gl/glfw.h"

#include "utils.h"
#include "cam.h";

Camera cam(kWidth, kHeight);

// Callback functions for GLFW window context
static void key_callback(int key, int action ,int mods) {
	std::cout<<"key="<<key<<std::endl;
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
	std::cout<<"mouse_x:"<<mouse_x<<",mouse_y:"<<mouse_y<<std::endl;
	cam.handleMouseMove(mouse_x, mouse_y);
}

int main(int argc, char** argv) {

	//Camera cam(kWidth,kHeight);

	// Init GLFW stuff
	glfwInit();
	if(!glfwOpenWindow(kWidth, kHeight, 8,8,8,8, 32, 0, GLFW_WINDOW)) {
		glfwTerminate();
		std::cout<<"Error init GLFW\n";
		return 0;
	}

	glClearColor(real(0),real(0),real(0),real(0));
	glfwSwapInterval(0);
	glfwSetKeyCallback((GLFWkeyfun)key_callback);
	glfwSetMousePosCallback((GLFWmouseposfun)handle_mouse_move);
	real* pixels = new real[3*kWidth*kHeight]();

	for(size_t i=0; i< 3*kWidth*kHeight; i += 3) {
		pixels[i + 0] = real(0.5);
		pixels[i + 1] = real(0.5);
		pixels[i + 2] = real(1.0);
	}

	float currentTime, lastTime = 0.0f;
	float deltaT = 0.01f;

	bool quit = false;
	while(!quit) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		currentTime = float(glfwGetTime());
		deltaT = currentTime - lastTime;
		lastTime = currentTime;

		cam.move(deltaT);

		// Render stuff here!
		glDrawPixels(kWidth, kHeight, GL_RGB, GL_FLOAT, pixels);

		// Swap buffers
		glfwSwapBuffers();
		//std::cout<<"Drawing!";
		if(glfwGetKey('Q') || !glfwGetWindowParam(GLFW_OPENED) ) {
			quit = true;
		}
	}
	glfwTerminate();
	return 0;
}
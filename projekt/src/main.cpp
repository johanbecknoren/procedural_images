#include <iostream>
#include "glew.h"
#include "gl/glfw.h"

#include <glm/gtc/type_ptr.hpp>

#include "utils.h"
#include "cam.h"
#include "fbo.h"
#include "terrain.h"
#include "loadobj.h"
#include "shadermanager.h"

Camera cam(kWidth, kHeight);

Terrain terrain;

void printError(const char *functionName)
{
	GLenum error;
	while (( error = glGetError() ) != GL_NO_ERROR)
	{
		fprintf (stderr, "GL error 0x%X detected in %s\n", error, functionName);
	}
}

// Callback functions for GLFW window context
static void key_callback(int key, int action ,int mods) {
	//std::cout<<"key="<<key<<std::endl;
	if(key == 'O') {
		if(action == GLFW_PRESS) {
			terrain.reloadShaders();
		}
	}
	if(key == 'V') {
		if(action == GLFW_PRESS) {
			terrain.toggleWireRender();
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
	} else if(key == GLFW_KEY_KP_ADD) {
		if(action == GLFW_PRESS) {
			terrain.updateNumOctaves(1);
		} 
	} else if(key == GLFW_KEY_KP_SUBTRACT) {
		if(action == GLFW_PRESS) {
			terrain.updateNumOctaves(-1);
		} 
	} else if(key == '+') {
		if(action == GLFW_PRESS) {
			terrain.updateWaterLevel(0.01f);
		} 
	} else if(key == '-') {
		if(action == GLFW_PRESS) {
			terrain.updateWaterLevel(-0.01f);
		} 
	}
}

bool first_time;

static void handle_mouse_move(int mouse_x, int mouse_y) {
	//std::cout<<"mouse_x:"<<mouse_x<<",mouse_y:"<<mouse_y<<std::endl;
	if(!first_time)
		cam.handleMouseMove(mouse_x, mouse_y);
	else
		first_time = false;
}

static void handle_mouse_button(int button, int action, int mods) {
	//std::cout << "Button: " << button << std::endl;
	if(button == 0) { // LMB
		if(action == GLFW_PRESS)
			cam.holdingLMB = true;
		else if(action == GLFW_RELEASE)
			cam.holdingLMB = false;
	} else if (button == 1) { // RMB
		if(action == GLFW_PRESS)
			cam.holdingRMB = true;
		else if(action == GLFW_RELEASE)
			cam.holdingRMB = false;
	}
}

char* lltoa(long long val, int base){

    static char buf[64] = {0};

    int i = 62;
    int sign = (val < 0);
    if(sign) val = -val;

    if(val == 0) return "0";

    for(; val && i ; --i, val /= base) {
        buf[i] = "0123456789abcdef"[val % base];
    }

    if(sign) {
        buf[i--] = '-';
    }
    return &buf[i+1];
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
	glfwSetMouseButtonCallback((GLFWmousebuttonfun)handle_mouse_button);

	float currentTime, lastTime = 0.0f;
	float deltaT = 0.01f;

	first_time = true;
	bool quit = false;
	high_resolution_clock::time_point c1;

	terrain.init();

	while(!quit) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		currentTime = float(glfwGetTime());
		deltaT = currentTime - lastTime;
		lastTime = currentTime;

		cam.move(deltaT);
		printError("Error moving camera");

		c1 = high_resolution_clock::now();
		terrain.render(cam.getModelView(), cam.getProjection(), cam.getPosition());
		auto elapsed = std::chrono::duration_cast<milliseconds>(high_resolution_clock::now() - c1);

		// Swap buffers
		glfwSwapBuffers();
		printError("Swap buffers");

		
		glfwSetWindowTitle( lltoa(elapsed.count(),10) );

		if(glfwGetKey('Q') || !glfwGetWindowParam(GLFW_OPENED) ) {
			quit = true;
		}
	}
	glfwTerminate();
	return 0;
}
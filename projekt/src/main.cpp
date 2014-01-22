#include <iostream>
#include "glew.h"
#include "gl/glfw.h"
#include "utils.h"

//#pragma comment(lib, "GLFW.lib")
//#pragma comment(lib, "opengl32.lib")
//#pragma comment(lib, "glu32.lib")

int main(int argc, char** argv) {

	glfwInit();
	if(!glfwOpenWindow(kWidth, kHeight, 8,8,8,8, 32, 0, GLFW_WINDOW)) {
		glfwTerminate();
		std::cout<<"Error init GLFW\n";
		return 0;
	}
	glClearColor(real(0),real(0),real(0),real(0));
	glfwSwapInterval(0);

	real* pixels = new real[3*kWidth*kHeight]();

	for(size_t i=0; i< 3*kWidth*kHeight; i += 3) {
		pixels[i + 0] = real(0.5);
		pixels[i + 1] = real(0.5);
		pixels[i + 2] = real(0.8);
	}

	bool quit = false;
	while(!quit) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDrawPixels(kWidth, kHeight, GL_RGB, GL_FLOAT, pixels);
		glfwSwapBuffers();
		//std::cout<<"Drawing!";
		if(glfwGetKey('Q') || !glfwGetWindowParam(GLFW_OPENED) ) {
			quit = true;
		}
	}
	glfwTerminate();
	char c;
	std::cin>>c;
	return 0;
}
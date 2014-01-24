#include "cam.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

// Borrowed from http://r3dux.org/2012/12/a-c-camera-class-for-simple-opengl-fps-controls/

const float Camera::TO_RADS = 3.141592654f / 180.0f; // The value of 1 degree in radians
const float M_PI = 3.14159f; 

Camera::Camera(int theWindowWidth, int theWindowHeight)
{
	initCamera();

	speedFactor = 0.1f;
 
	windowWidth  = theWindowWidth;
	windowHeight = theWindowHeight;
 
	// Calculate the middle of the window
	windowMidX = windowWidth  / 2;
	windowMidY = windowHeight / 2;

	projection = glm::perspective(45.0f, (float)windowWidth / (float)windowHeight, 1.0f, 200.0f);

	//glfwSetCursorPos(window, windowMidX, windowMidY);
	glfwSetMousePos(windowMidX,windowMidY);
}
 
Camera::~Camera()
{
	// Nothing to do here - we don't need to free memory as all member variables
	// were declared on the stack.
}
 
void Camera::initCamera()
{
	// Set position, rotation and speed values to zero
	position = glm::vec3(0.5f, 0.5f, 4.f);
	rotation = glm::vec3(0.f);
	speed = glm::vec3(0);
 
	// How fast we move (higher values mean we move and strafe faster)
	movementSpeedFactor = 100.0f;
 
	pitchSensitivity = 0.2f; // How sensitive mouse movements affect looking up and down
	yawSensitivity   = 0.2f; // How sensitive mouse movements affect looking left and right
 
	// To begin with, we aren't holding down any keys
	holdingForward     = false;
	holdingBackward    = false;
	holdingLeftStrafe  = false;
	holdingRightStrafe = false;
}
 
// Function to convert degrees to radians
const float Camera::toRads(const float &theAngleInDegrees) const
{
	return theAngleInDegrees * TO_RADS;
}
 
// Function to deal with mouse position changes
void Camera::handleMouseMove(int mouseX, int mouseY)
{
	// Calculate our horizontal and vertical mouse movement from middle of the window
	float horizMovement = float(mouseX - windowMidX+1) * yawSensitivity;
	float vertMovement  = float(mouseY - windowMidY) * pitchSensitivity;
 
	/*std::cout << "Mid window values: " << windowMidX << "\t" << windowMidY << std::endl;
	std::cout << "Mouse values     : " << mouseX << "\t" << mouseY << std::endl;
	std::cout << horizMovement << "\t" << vertMovement << std::endl << std::endl;*/
 
	// Apply the mouse movement to our rotation vector. The vertical (look up and down)
	// movement is applied on the X axis, and the horizontal (look left and right)
	// movement is applied on the Y Axis
	rotation.x += (vertMovement);
	rotation.y += (horizMovement);
 
	// Limit loking up to vertically up
	if (rotation.x < -90)
	{
		rotation.x = (-90);
	}
 
	// Limit looking down to vertically down
	if (rotation.x > 90)
	{
		rotation.x = (90);
	}
 
	// If you prefer to keep the angles in the range -180 to +180 use this code
	// and comment out the 0 to 360 code below.
	//
	// Looking left and right. Keep the angles in the range -180.0f (anticlockwise turn looking behind) to 180.0f (clockwise turn looking behind)
	/*if (yRot < -180.0f)
	{
	    yRot += 360.0f;
	}
 
	if (yRot > 180.0f)
	{
	    yRot -= 360.0f;
	}*/
 
	// Looking left and right - keep angles in the range 0.0 to 360.0
	// 0 degrees is looking directly down the negative Z axis "North", 90 degrees is "East", 180 degrees is "South", 270 degrees is "West"
	// We can also do this so that our 360 degrees goes -180 through +180 and it works the same, but it's probably best to keep our
	// range to 0 through 360 instead of -180 through +180.
	if (rotation.y < 0)
	{
		rotation.y += (360);
	}
	if (rotation.y > 360)
	{
		rotation.y += (-360);
	}
 
	// Reset the mouse position to the centre of the window each frame
	//glfwSetCursorPos(glfwGetWindowSize(), windowMidX, windowMidY);
	glfwSetMousePos(windowMidX, windowMidY);
}
#include <glm/gtc/matrix_transform.hpp>
glm::mat4 Camera::getModelView() {
	glm::mat4 view;
	view = glm::rotate(view, rotation.x, glm::vec3(1.f, 0.f, 0.f));
	view = glm::rotate(view, rotation.y, glm::vec3(0.f, 1.f, 0.f));
	//view = glm::rotate(view, rotation.z, glm::vec3(0.f, 0.f, 1.f));
	view = glm::translate(view, -position);
	/*view = glm::rotate(view, rotation.x, glm::vec3(1.f, 0.f, 0.f));
	view = glm::rotate(view, rotation.y, glm::vec3(0.f, 1.f, 0.f));
	view = glm::translate(view, -position);*/
	return view;
}

// Function to calculate which direction we need to move the camera and by what amount
void Camera::move(float deltaTime)
{
	// Vector to break up our movement into components along the X, Y and Z axis
	glm::vec3 movement = glm::vec3(0);
 
	// Get the sine and cosine of our X and Y axis rotation
	float sinXRot = sin( toRads( rotation.x ) );
	float cosXRot = cos( toRads( rotation.x ) );
 
	float sinYRot = sin( toRads( rotation.x ) );
	float cosYRot = cos( toRads( rotation.x ) );
 
	float pitchLimitFactor = cosXRot; // This cancels out moving on the Z axis when we're looking up or down
 
	if (holdingForward)
	{
		movement.x +=(sinYRot * pitchLimitFactor);
		movement.y +=(-sinXRot);
		movement.z +=(-cosYRot * pitchLimitFactor);
	}
 
	if (holdingBackward)
	{
		movement.x +=(-sinYRot * pitchLimitFactor);
		movement.y +=(sinXRot);
		movement.z +=(cosYRot * pitchLimitFactor);
	}
 
	if (holdingLeftStrafe)
	{
		movement.x +=(-cosYRot);
		movement.z +=(-sinYRot);
	}
 
	if (holdingRightStrafe)
	{
		movement.x +=(cosYRot);
		movement.z +=(sinYRot);
	}
 
	// Normalise our movement vector
	if (glm::length(movement) > 0)
		movement = glm::normalize(movement);
 
	// Calculate our value to keep the movement the same speed regardless of the framerate...
	float framerateIndependentFactor = movementSpeedFactor * (deltaTime * speedFactor);
 
	// .. and then apply it to our movement vector.
	movement *= framerateIndependentFactor;
 
	// Finally, apply the movement to our position
	position += movement;
}
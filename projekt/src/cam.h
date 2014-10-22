#ifndef CAMERA_H
#define CAMERA_H
 
#include <iostream>
#include <math.h>         // Used only for sin() and cos() functions
 
#include <GL/glfw.h>     // Include OpenGL Framework library for the GLFW_PRESS constant only!
 
#include <glm/glm.hpp>

#include "utils.h"
 
// BORROWED FROM http://r3dux.org/2012/12/a-c-camera-class-for-simple-opengl-fps-controls/

class Camera
{
    protected:
        // Camera position
		glm::vec3 position;
 
        // Camera rotation
        glm::vec3 rotation;
 
        // Camera movement speed. When we call the move() function on a camera, it moves using these speeds
        glm::vec3 speed;
 
		glm::mat4 projection;

        float movementSpeedFactor; // Controls how fast the camera moves
        float pitchSensitivity;    // Controls how sensitive mouse movements affect looking up and down
        float yawSensitivity;      // Controls how sensitive mouse movements affect looking left and right
 
        // Window size in pixels and where the midpoint of it falls
        int windowWidth;
        int windowHeight;
        int windowMidX;
        int windowMidY;
 
        // Method to set some reasonable default values. For internal use by the class only.
        void initCamera();

    public:
        static const float TO_RADS; // The value of 1 degree in radians

		float speedFactor;
 
        // Holding any keys down?
        bool holdingForward;
        bool holdingBackward;
        bool holdingLeftStrafe;
        bool holdingRightStrafe;
		bool holdingLMB;
		bool holdingRMB;
 
        // Constructors
        Camera(int windowWidth, int windowHeight);
 
        // Destructor
        ~Camera();
 
        // Mouse movement handler to look around
        void handleMouseMove(int mouseX, int mouseY);
 
        // Method to convert an angle in degress to radians
        const float toRads(const float &angleInDegrees) const;
 
        // Method to move the camera based on the current direction
        void move(float deltaTime);
 
		glm::mat4 getProjection() { return projection; };
		glm::mat4 getModelView();

		void print() { std::cout << position.x << " " << position.y << " " << position.z << std::endl; };

        // --------------------------------- Inline methods ----------------------------------------------
 
        // Setters to allow for change of vertical (pitch) and horizontal (yaw) mouse movement sensitivity
        float getPitchSensitivity()            { return pitchSensitivity;  }
        void  setPitchSensitivity(float value) { pitchSensitivity = value; }
        float getYawSensitivity()              { return yawSensitivity;    }
        void  setYawSensitivity(float value)   { yawSensitivity   = value; }
 
        // Position getters
        glm::vec3 getPosition() const { return position;        }
        float getXPos()           const { return position.x; }
        float getYPos()           const { return position.y; }
        float getZPos()           const { return position.z; }
 
        // Rotation getters
		glm::vec3 getRotation() const { return rotation;        }
        float getXRot()           const { return rotation.x; }
        float getYRot()           const { return rotation.y; }
        float getZRot()           const { return rotation.z; }
};
 
#endif // CAMERA_H
#ifndef UTILS_H
#define UTILS_H
#include "glm/glm.hpp"
#include <stdint.h>
#include <chrono>

#define CMAKE_PROJECT_ROOT_DIR "C:/code/Other/procedural_images/projekt"//"G:/LiU/Ak5/Procedurella/git-repot/procedural_images/projekt"
//#define CMAKE_PROJECT_ROOT_DIR "G:/LiU/Ak5/Procedurella/git-repot/procedural_images/projekt"

// Constants
const int kWidth = 1280;
const int kHeight = 720;

const unsigned int kGridWidth = 800;
const unsigned int kGridHeight = 800;
const float kGridPointSpacing = .05f;

const float M_PI = 3.14159f; 

// Typedefs
typedef float real;
typedef glm::vec3 Vec3;
typedef unsigned int uint;

typedef std::chrono::high_resolution_clock high_resolution_clock;
typedef std::chrono::milliseconds milliseconds;
typedef std::chrono::microseconds microseconds;
#endif
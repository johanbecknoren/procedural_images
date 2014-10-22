#ifndef UTILS_H
#define UTILS_H
#include "glm/glm.hpp"
#include <stdint.h>

#define CMAKE_PROJECT_ROOT_DIR "E:/procedurella/procedural_images/projekt"//"G:/LiU/Ak5/Procedurella/git-repot/procedural_images/projekt"
//#define CMAKE_PROJECT_ROOT_DIR "G:/LiU/Ak5/Procedurella/git-repot/procedural_images/projekt"

// Constants
const int kWidth = 1280;
const int kHeight = 720;

const unsigned int kGridWidth = 80;
const unsigned int kGridHeight = 80;
const unsigned int kGridPointSpacing = 1;

const float M_PI = 3.14159f; 

// Typedefs
typedef float real;
typedef glm::vec3 Vec3;
typedef unsigned int uint;
#endif
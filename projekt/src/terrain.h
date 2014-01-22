#ifndef TERRAIN_H
#define TERRAIN_H

#include "utils.h"

class Terrain {
public:
	Terrain();
	~Terrain() {};

	void init();
	void render(const glm::mat4 &MV, const glm::mat4 &proj);
};
#endif TERRAIN_H
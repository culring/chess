#pragma once
#include <glm/mat4x2.hpp>
#include <vector>
#include "Mesh.h"

class Shader;

class QueenModel
{
	const float BOTTOM_CUBOID_SIDE = 4.0f,
		BOTTOM_CUBOID_HEIGHT = 0.25f * BOTTOM_CUBOID_SIDE,
		TOP_CUBOID_SIDE = 0.50f * BOTTOM_CUBOID_SIDE,
		TOP_CUBOID_HEIGHT = 0.25f * BOTTOM_CUBOID_SIDE,
		CONE_RADIUS = 0.3125f * BOTTOM_CUBOID_SIDE,
		CONE_HEIGHT = BOTTOM_CUBOID_SIDE,
		SPHERE_RADIUS = 0.25f * BOTTOM_CUBOID_SIDE;
	const char* TEXTURE_PATHS[2];

	std::vector<Mesh> meshes;
	std::vector<glm::mat4> translations;
	std::vector<GLuint> textures;

public:
	QueenModel();
	void draw(Shader shader, unsigned int modelLoc, glm::mat4 trans);
};


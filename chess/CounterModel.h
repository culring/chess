#pragma once
#include "Mesh.h"
#include <glm/mat4x2.hpp>

class CounterModel
{
	const float CYLINDER_RADIUS = 4.0f / 2,
		CYLINDER_HEIGHT = 0.5f*CYLINDER_RADIUS,
		CONE_RADIUS = 0.625f*CYLINDER_RADIUS,
		CONE_HEIGHT = 2.0f*CYLINDER_RADIUS,
		SPHERE_RADIUS = 0.5f*CYLINDER_RADIUS;
	const char* TEXTURE_PATHS[1];

	std::vector<Mesh> meshes;
	std::vector<glm::mat4> translations;
	std::vector<GLuint> textures;

public:
	CounterModel();
	void draw(Shader shader, unsigned int modelLoc, glm::mat4 trans);
	// side:
	// 0 - up
	// 1- right
	// 2 - down
	// 3 - left
	glm::vec3 getAnchor(int side);
	glm::vec3 getAnchor2(int side);
	float getAngle();
};


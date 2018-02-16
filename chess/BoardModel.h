#pragma once
#include <vector>
#include <glm/mat4x2.hpp>
#include "Mesh.h"

class BoardModel
{
	std::vector<Mesh> meshes;
	std::vector<GLuint> textures;
	const char* TEXTURE_PATHS[3];
	const unsigned int NUMBER_OF_FIELDS = 8;
	const float BORDER_TO_FIELD_RATIO = 41.0f / 80.0f;
	const float FIELD_SIZE = 4.0f;
	const float BORDER_SIZE = FIELD_SIZE * BORDER_TO_FIELD_RATIO;
	const float BOARD_SIZE = NUMBER_OF_FIELDS*FIELD_SIZE + BORDER_SIZE*2,
		HEIGHT = 2.5f;
//	const float FIELD_SIZE = 4.0f, 
//		BOARD_SIZE = 10*FIELD_SIZE,
//		HEIGHT = 2.5f;

public:
	BoardModel();
	void draw(Shader shader, unsigned int modelLoc, glm::mat4 trans);
	glm::vec3 getPositionOfField(int x, int y);
	glm::vec3 getPositionOfField(std::pair<int, int> position);
	float getSizeOfField();
};


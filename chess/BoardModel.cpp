#include "BoardModel.h"
#include "Texturer.h"
#include <glm/detail/type_mat4x4.hpp>
#include "MeshGenerator.h"
#include <glm/gtc/matrix_transform.inl>
#include <iostream>


BoardModel::BoardModel()
{
	TEXTURE_PATHS[0] = "textures/wood_2.jpg";
//	TEXTURE_PATHS[1] = "textures/checkerboard.png";
//	TEXTURE_PATHS[1] = "textures/wooden_board_2.jpg";
	TEXTURE_PATHS[1] = "textures/wooden_board.png";
	TEXTURE_PATHS[2] = "textures/suede_3.jpg";
	textures.emplace_back(Texturer::loadMipmapTexture(TEXTURE_PATHS[0]));
	textures.emplace_back(Texturer::loadMipmapTexture(TEXTURE_PATHS[1]));
	textures.emplace_back(Texturer::loadMipmapTexture(TEXTURE_PATHS[2]));

	meshes.emplace_back(MeshGenerator::createCuboid(BOARD_SIZE, BOARD_SIZE, HEIGHT));
}

void BoardModel::draw(Shader shader, unsigned modelLoc, glm::mat4 trans)
{
	trans = glm::translate(trans, glm::vec3(0.0f, -HEIGHT / 2, 0.0f));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	meshes[0].drawSideFaces(shader, modelLoc, trans);

	glBindTexture(GL_TEXTURE_2D, textures[2]);
	meshes[0].drawBottomFace(shader, modelLoc, trans);

	glBindTexture(GL_TEXTURE_2D, textures[1]);
	meshes[0].drawTopFace(shader, modelLoc, trans);
}

glm::vec3 BoardModel::getPositionOfField(int x, int y)
{
	if(x >= NUMBER_OF_FIELDS || y >= NUMBER_OF_FIELDS || x < 0 || y < 0)
	{
		throw std::exception("Tried to access checkerboard field beyond allowed ranges");
	}

	glm::vec3 trans(
		BORDER_SIZE + FIELD_SIZE / 2 + x*FIELD_SIZE, 
		0.0001f, 
		BORDER_SIZE + FIELD_SIZE / 2 + y*FIELD_SIZE
	);
	trans += glm::vec3(-BOARD_SIZE / 2, 0.0f, -BOARD_SIZE / 2);

	return trans;
}

glm::vec3 BoardModel::getPositionOfField(std::pair<int, int> position)
{
	return getPositionOfField(position.first, position.second);
}

float BoardModel::getSizeOfField()
{
	return FIELD_SIZE;
}

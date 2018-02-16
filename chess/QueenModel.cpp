#include "QueenModel.h"
#include "MeshGenerator.h"
#include <glm/gtc/matrix_transform.inl>
#include "Texturer.h"

QueenModel::QueenModel()
{
	TEXTURE_PATHS[0] = "textures/white_gray_granite.jpg";
	textures.emplace_back(Texturer::loadMipmapTexture(TEXTURE_PATHS[0]));

	meshes.push_back(MeshGenerator::createCone(CONE_RADIUS, CONE_HEIGHT));
	meshes.push_back(MeshGenerator::createSphere(SPHERE_RADIUS));
	meshes.push_back(MeshGenerator::createCuboid(TOP_CUBOID_SIDE, TOP_CUBOID_SIDE, TOP_CUBOID_HEIGHT));
	meshes.push_back(MeshGenerator::createCuboid(BOTTOM_CUBOID_SIDE, BOTTOM_CUBOID_SIDE, BOTTOM_CUBOID_HEIGHT));

	translations.emplace_back();
	translations.push_back(glm::translate(glm::mat4(), glm::vec3(0.0f, CONE_HEIGHT / 2, 0.0f)));
	translations.push_back(glm::translate(glm::mat4(), glm::vec3(0.0f, (CONE_HEIGHT+SPHERE_RADIUS+TOP_CUBOID_HEIGHT) / 2, 0.0f)));
	translations.push_back(glm::translate(glm::mat4(), glm::vec3(0.0f, -(CONE_HEIGHT+BOTTOM_CUBOID_HEIGHT) / 2, 0.0f)));
}

void QueenModel::draw(Shader shader, unsigned modelLoc, glm::mat4 trans)
{
	trans = glm::translate(trans, glm::vec3(0.0f, CONE_HEIGHT / 2 + BOTTOM_CUBOID_HEIGHT, 0.0f));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);

	for(int i = 0; i < meshes.size(); ++i)
	{
		glm::mat4 mat = trans*translations[i];
		meshes[i].draw(shader, modelLoc, mat);
	}
}

#include "CounterModel.h"
#include "MeshGenerator.h"
#include <glm/gtc/matrix_transform.inl>
#include <glm/gtc/type_ptr.inl>
#include "Texturer.h"
#include <iostream>

CounterModel::CounterModel()
{
	TEXTURE_PATHS[0] = "textures/black_granite.jpg";
	textures.emplace_back(Texturer::loadMipmapTexture(TEXTURE_PATHS[0]));

	meshes.push_back(MeshGenerator::createCone(CONE_RADIUS, CONE_HEIGHT));
	meshes.push_back(MeshGenerator::createSphere(SPHERE_RADIUS));
	meshes.push_back(MeshGenerator::createCylinder(CYLINDER_RADIUS, CYLINDER_HEIGHT));

	translations.emplace_back();
	translations.push_back(glm::translate(glm::mat4(), glm::vec3(0.0f, CONE_HEIGHT/2, 0.0f)));
	translations.push_back(glm::translate(glm::mat4(), glm::vec3(0.0f, -(CONE_HEIGHT+CYLINDER_HEIGHT)/2, 0.0f)));
}

void CounterModel::draw(Shader shader, unsigned modelLoc, glm::mat4 trans)
{
	trans = glm::translate(trans, glm::vec3(0.0f, CONE_HEIGHT / 2 + CYLINDER_HEIGHT, 0.0f));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);

	for(int i = 0; i < meshes.size(); ++i)
	{
		meshes[i].draw(shader, modelLoc, trans*translations[i]);
	}
}

glm::vec3 CounterModel::getAnchor(int side)
{
	glm::vec3 vec;
	switch(side)
	{
	case 0:
		vec = glm::vec3(0.0f, 0.0f, -CYLINDER_RADIUS);
		break;
	case 1:
		vec = glm::vec3(CYLINDER_RADIUS, 0.0f, 0.0f);
		break;
	case 2:
		vec = glm::vec3(0.0f, 0.0f, CYLINDER_RADIUS);
		break;
	case 3:
		vec = glm::vec3(-CYLINDER_RADIUS, 0.0f, 0.0f);
		break;
	default:
		throw std::exception();
	}
	return vec;
}

glm::vec3 CounterModel::getAnchor2(int side)
{
	glm::vec3 vec;
	switch(side)
	{
	case 0:
		vec = glm::vec3(0.0f, CYLINDER_HEIGHT, -CYLINDER_RADIUS);
		break;
	case 1:
		vec = glm::vec3(CYLINDER_RADIUS, CYLINDER_HEIGHT, 0.0f);
		break;
	case 2:
		vec = glm::vec3(0.0f, CYLINDER_HEIGHT, CYLINDER_RADIUS);
		break;
	case 3:
		vec = glm::vec3(-CYLINDER_RADIUS, CYLINDER_HEIGHT, 0.0f);
		break;
	default:
		throw std::exception();
	}
	return vec;
}

float CounterModel::getAngle()
{
	return glm::degrees(glm::atan((CYLINDER_RADIUS-SPHERE_RADIUS)/CONE_HEIGHT));
}

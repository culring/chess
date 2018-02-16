#include "SkyboxModel.h"
#include "Texturer.h"
#include "MeshGenerator.h"
#include <glm/detail/type_mat4x4.hpp>
#include <iostream>

SkyboxModel::SkyboxModel()
{
	filenames = {
		"rt",
		"lf",
		"up",
		"dn",
		"ft",
		"bk"
	};
	for(int i = 0; i<filenames.size(); ++i)
	{
		filenames[i] = DIRECTORY + PACKAGE_NAME + filenames[i] + EXTENSION;
		std::cout << filenames[i] << std::endl;
	}
	try
	{
		cubemapTexture = Texturer::loadCubemapTexture(filenames);
	}
	catch(std::exception e)
	{
		std::cout << e.what() << std::endl;
	}
	meshes.emplace_back(MeshGenerator::createCuboid(500.0f, 500.0f, 500.0f));
}

void SkyboxModel::draw(Shader shader)
{
	shader.Use();
	glDepthMask(GL_FALSE);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDepthFunc(GL_LEQUAL);
	meshes[0].draw(shader, 3, glm::mat4());
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);
}

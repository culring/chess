#pragma once
#include <vector>
#include "Mesh.h"

class SkyboxModel
{
	const std::string DIRECTORY = "textures/skybox/mp_dejavu/";
	const std::string PACKAGE_NAME = "dejavu_";
	const std::string EXTENSION = ".jpg";
	std::vector<std::string> filenames;
	unsigned int cubemapTexture;
	std::vector<Mesh> meshes;

public:
	SkyboxModel();;

	void draw(Shader shader);
};


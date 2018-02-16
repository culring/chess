#pragma once
#include <GL/glew.h>
#include <vector>

class Texturer
{
public:
	static GLuint loadMipmapTexture(const char* fname);
	static GLuint loadCubemapTexture(std::vector<std::string> filenames);
};


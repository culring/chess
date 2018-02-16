#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

class Shader
{
	GLuint program_id; 	// The program ID
public:
	// Constructor reads and builds the shader
	Shader(const char* vertexPath, const char* fragmentPath);

	// Use the program
	void Use() const
	{
		glUseProgram(get_programID());
	}

	// returns program ID
	GLuint get_programID() const
	{
		return program_id;
	}
};

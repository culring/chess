#pragma once
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "Camera.h"
#include "QueenModel.h"
#include "CounterModel.h"
#include "BoardModel.h"
#include "Checker.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

class Game
{
public:
	Game();
	void run();

private:
	GLFWwindow* m_window;
	Shader *shaderProgram;

	void initGLFW();
	void initOpenGL();
};


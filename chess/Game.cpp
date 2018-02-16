#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Game.h"
#include <iostream>
#include "Mesh.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "CounterModel.h"
#include "QueenModel.h"
#include "BoardModel.h"
#include "Camera.h"
#include "Animator.h"
#include "Checker.h"
#include "SkyboxModel.h"

// settings
const unsigned int SCR_WIDTH = 1080;
const unsigned int SCR_HEIGHT = 720;

// camera
Camera camera(
	glm::vec3(36.7879, 11.3766, 44.1112),
	glm::vec3(-0.0336259, 0.998806, -0.0354343),
	-853.5,
	-2.79999
);
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
bool isMousePressed = false;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

//animation
//float beatingAnimationTime = 2.5f;
float beatingAnimationTime = 2.0f;
float beatingSpeed = 1.0f / beatingAnimationTime;
float movingAnimationTime = 0.7f;
float movingSpeed = 1.0f / movingAnimationTime;

// dynamic animation
enum Move
{
	MOVE_LEFT,
	MOVE_RIGHT,
	MOVE_UP,
	MOVE_DOWN,
	NO_MOVE
};
Move queenMove = NO_MOVE;
enum Animating
{
	NOT_ANIMATING,
	MOVING,
	JUMPING
};
Animating queenAnimating = NOT_ANIMATING;
enum GameState
{
	RUNNING,
	ENDED
};
GameState gameState = RUNNING;
std::pair<int, int> initialPosition;
std::pair<int, int> destinationPosition;
int side;

Game::Game()
{

}

void Game::run()
{
	// init
	initGLFW();
	initOpenGL();

	// shader stuff
	Shader shader("basic_shader.vert", "basic_shader.frag"),
		skyboxShader("skybox_shader.vert", "skybox_shader.frag");

	// camera
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 30.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, -1.0f);

	// models
	CounterModel counterModel;
	QueenModel queenModel;
	BoardModel boardModel;
	SkyboxModel skybox;
	// information about positions of checkers
	int queenPositionX = 5, queenPositionY = 6,
		counterPositionX = 6, counterPositionY = 5;
	Checker queenChecker(queenPositionX, queenPositionY),
		counterChecker(counterPositionX, counterPositionY);

	float t = 0;
	glm::mat4 counterTrans, queenTrans;

	queenTrans = glm::translate(glm::mat4(), boardModel.getPositionOfField(
		queenChecker.getPositionX(), queenChecker.getPositionY()
	));
	counterTrans = glm::translate(glm::mat4(), boardModel.getPositionOfField(
		counterChecker.getPositionX(), counterChecker.getPositionY()
	));

	while(!glfwWindowShouldClose(m_window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		if(lastFrame == 0)
		{
			lastFrame = currentFrame;
		}
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(m_window);

		// Clear the colorbuffer
		glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)SCR_WIDTH / (GLfloat)SCR_HEIGHT, 1.0f, 1000.0f);
		glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix()));

		skyboxShader.Use();
		GLuint viewLoc = glGetUniformLocation(skyboxShader.get_programID(), "view");
		GLuint projLoc = glGetUniformLocation(skyboxShader.get_programID(), "projection");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		shader.Use();
		GLuint modelLoc = glGetUniformLocation(shader.get_programID(), "model");
		viewLoc = glGetUniformLocation(shader.get_programID(), "view");
		projLoc = glGetUniformLocation(shader.get_programID(), "projection");
		// set perspective projection & pass it to GPU
//			glm::mat4 projection = glm::perspective(glm::radians(2*fov), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		// set view transformation & pass it to GPU
		view = camera.GetViewMatrix();
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		// queen hasn't beaten counter yet
		if(gameState == RUNNING)
		{
			// queen checker currently not animating
			if(queenAnimating == NOT_ANIMATING)
			{
				// check for user input
				if(queenMove != NO_MOVE)
				{
					initialPosition = queenChecker.getPosition();
					destinationPosition;
					if(queenMove == MOVE_LEFT)
					{
						queenChecker.moveLeft();
						if(counterChecker.getPosition() == queenChecker.getPosition())
						{
							queenChecker.moveLeft();
							queenAnimating = JUMPING;
							if(queenChecker.getPositionY() > 1)
							{
								side = 0;
							}
							else side = 2;
						}
					}
					else if(queenMove == MOVE_RIGHT)
					{
						queenChecker.moveRight();
						if(counterChecker.getPosition() == queenChecker.getPosition())
						{
							queenChecker.moveRight();
							queenAnimating = JUMPING;
							if(queenChecker.getPositionY() > 1)
							{
								side = 0;
							}
							else side = 2;
						}
					}
					else if(queenMove == MOVE_UP)
					{
						queenChecker.moveUp();
						if(counterChecker.getPosition() == queenChecker.getPosition())
						{
							queenChecker.moveUp();
							queenAnimating = JUMPING;
							if(queenChecker.getPositionX() > 1)
							{
								side = 3;
							}
							else side = 1;
						}
					}
					else if(queenMove == MOVE_DOWN)
					{
						queenChecker.moveDown();
						if(counterChecker.getPosition() == queenChecker.getPosition())
						{
							queenChecker.moveDown();
							queenAnimating = JUMPING;
							if(queenChecker.getPositionX() > 1)
							{
								side = 3;
							}
							else side = 1;
						}
					}
					destinationPosition = queenChecker.getPosition();
					if(queenAnimating != JUMPING && initialPosition != destinationPosition)
					{
						queenAnimating = MOVING;
					}
					queenMove = NO_MOVE;
				}
			}
			// queen currently animating
			else
			{
				if(queenAnimating == MOVING)
				{
					if(t >= 1.0f)
					{
						queenAnimating = NOT_ANIMATING;
						t = 0.0f;
					}
					else
					{
						queenTrans = Animator::getMoveFrame(
							boardModel.getPositionOfField(initialPosition),
							boardModel.getPositionOfField(destinationPosition),
							t
						);
					}
					t += deltaTime*movingSpeed;
				}
				else if(queenAnimating == JUMPING)
				{
					if(t >= 1.0f)
					{
						queenAnimating = NOT_ANIMATING;
						t = 0.0f;
						gameState = ENDED;
					}
					else
					{
						counterTrans = glm::translate(glm::mat4(), boardModel.getPositionOfField(
							counterChecker.getPositionX(), counterChecker.getPositionY()
						));

						// checker frame
						counterTrans *= Animator::getCollapseFrame(
							counterModel.getAnchor(side),
							counterModel.getAnchor2(side),
							counterModel.getAngle(),
							side,
							t
						);

						// queen frame
						queenTrans = Animator::getJumpFrame(
							boardModel.getPositionOfField(initialPosition),
							boardModel.getPositionOfField(destinationPosition),
							6.5f,
							t
						);
					}
					t += deltaTime*beatingSpeed;
				}
			}
		}

		counterModel.draw(shader, modelLoc, counterTrans);
		queenModel.draw(shader, modelLoc, queenTrans);
		boardModel.draw(shader, modelLoc, glm::mat4());
		skybox.draw(skyboxShader);

		// Swap the screen buffers
		glfwSwapBuffers(m_window);
	}

	glfwTerminate();
}

void Game::initGLFW()
{
	if (glfwInit() != GL_TRUE)
	{
		throw std::exception("GLFW Initialization failed");
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	try
	{
		m_window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Chess", nullptr, nullptr);
		if (m_window == nullptr)
			throw std::exception("GLFW window not created");
		glfwMakeContextCurrent(m_window);

		//input
		glfwSetCursorPosCallback(m_window, mouse_callback);
		// tell GLFW to capture our mouse
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
			throw std::exception("GLEW Initialization failed");
	}
	catch (std::exception ex)
	{
		std::cout << ex.what() << std::endl;
	}
}

void Game::initOpenGL()
{
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	// Set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_MIPMAP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glEnable(GL_DEPTH_TEST); // let's use z-buffer
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);

	if(gameState == RUNNING && queenAnimating == NOT_ANIMATING)
	{
		if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			queenMove = MOVE_LEFT;
		if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			queenMove = MOVE_RIGHT;
		if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			queenMove = MOVE_UP;
		if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			queenMove = MOVE_DOWN;
	}
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if(firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
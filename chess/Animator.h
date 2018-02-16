#pragma once
#include <glm/glm.hpp>

class Animator
{
	static glm::vec3 getAxisFromSide(int side);
public:
	static glm::mat4 getMoveFrame(glm::vec3 start, glm::vec3 end, float t);
	static glm::mat4 getJumpFrame(glm::vec3 start, glm::vec3 end, float height, float t);
	static glm::mat4 getCollapseFrame(glm::vec3 anchor, glm::vec3 anchor2, float angle, int side, float t);
};


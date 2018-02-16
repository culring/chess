#include "Animator.h"
#include <glm/gtc/matrix_transform.inl>
#include "AnimationParameterException.h"
#include <ostream>
#include <iostream>

glm::vec3 Animator::getAxisFromSide(int side)
{
	switch(side)
	{
	case 0:
		return glm::vec3(1.0f, 0.0f, 0.0f);
	case 1:
		return glm::vec3(0.0f, 0.0f, 1.0f);
	case 2:
		return glm::vec3(-1.0f, 0.0f, 0.0f);
	case 3:
		return glm::vec3(0.0f, 0.0f, -1.0f);
	default:
		throw std::exception();
	}
}

glm::mat4 Animator::getMoveFrame(glm::vec3 start, glm::vec3 end, float t)
{
	if(t < 0 || t > 1)
	{
		throw new AnimationParameterException();
	}
	return glm::translate(glm::mat4(), (end - start)*t + start);
}

glm::mat4 Animator::getJumpFrame(glm::vec3 start, glm::vec3 end, float height, float t)
{
	if(t < 0 || t > 1)
	{
		throw new AnimationParameterException();
	}

	const glm::vec3 middlePoint1(start.x, start.y + height, start.z),
		middlePoint2(end.x, end.y + height, end.z);
	if(t <= 1.0f/3.0f)
	{
		return getMoveFrame(start, middlePoint1, t / (1.0f / 3.0f));
	}
	if(t <= 2.0f/3.0f)
	{
		return getMoveFrame(middlePoint1, middlePoint2, (t - (1.0f / 3.0f)) / (1.0f / 3.0f));
	}
	// t e (2/3; 1]
	return getMoveFrame(middlePoint2, end, (t - 2.0f/3.0f) / (1.0f/3.0f));
}

glm::mat4 Animator::getCollapseFrame(glm::vec3 anchor, glm::vec3 anchor2, float angle, int side, float t)
{
	if(t < 0 || t > 1)
	{
		throw new AnimationParameterException();
	}
	
	glm::vec3 axis = getAxisFromSide(side);

	glm::mat4 trans;

	float currentAngle = (90.0f + angle)*t*t*t;

	if(currentAngle <= 90.0f)
	{
		trans *= glm::translate(glm::mat4(), anchor);
		trans *= glm::rotate(glm::mat4(), glm::radians(-currentAngle), axis);
		trans *= glm::translate(glm::mat4(), -anchor);
	}
	else
	{
		trans *= glm::translate(glm::mat4(), anchor);
		trans *= glm::rotate(glm::mat4(), glm::radians(-90.0f), axis);
		trans *= glm::translate(glm::mat4(), -anchor);

		trans *= glm::translate(glm::mat4(), anchor2);
		trans *= glm::rotate(glm::mat4(), glm::radians(-(currentAngle-90.0f)), axis);
		trans *= glm::translate(glm::mat4(), -anchor2);
	}

	return trans;
}

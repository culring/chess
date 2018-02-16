#include "AnimationParameterException.h"

AnimationParameterException::AnimationParameterException() : runtime_error("Animation parameter beyound bounds [0; 1]")
{
}

const char* AnimationParameterException::what() const
{
	return "Animation parameter beyound bounds [0; 1]";
}

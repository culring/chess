#pragma once
#include <stdexcept>

class AnimationParameterException :
	public std::runtime_error
{
public:
	AnimationParameterException();
	const char* what() const override;
};


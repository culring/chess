#pragma once
#include <utility>

class Checker
{
	int m_x = 0, m_y = 0;

public:
	Checker();
	Checker(int x, int y);

	int getPositionX();
	int getPositionY();
	std::pair<int, int> getPosition();
	void setPositionX(int x);
	void setPositionY(int y);
	void setPosition(int x, int y);
	void moveUp();
	void moveRight();
	void moveDown();
	void moveLeft();
};


#include "Checker.h"

Checker::Checker()
{
}

Checker::Checker(int x, int y) : m_x(x), m_y(y)
{
}

int Checker::getPositionX()
{
	return m_x;
}

int Checker::getPositionY()
{
	return m_y;
}

std::pair<int, int> Checker::getPosition()
{
	return{ m_x, m_y };
}

void Checker::setPositionX(int x)
{
	m_x = x;
}

void Checker::setPositionY(int y)
{
	m_y = y;
}

void Checker::setPosition(int x, int y)
{
	m_x = x, m_y = y;
}

void Checker::moveUp()
{
	if(m_y > 0) --m_y;;
}

void Checker::moveRight()
{
	if(m_x < 7) ++m_x;
}

void Checker::moveDown()
{
	if(m_y < 7) ++m_y;
}

void Checker::moveLeft()
{
	if(m_x > 0) --m_x;
}

#include "stdafx.h"
#include "Tile.h"

Tile::Tile(float x, float y, float size, bool marked) : m_x(x), m_y(y), m_isOccupied(marked), m_size(size), m_colour(255, 255, 255, 255)
{
}

void Tile::isOccupied(bool marked)
{
	m_lock.lock();
	m_isOccupied = marked; //critical section
	m_lock.unlock();
}

bool Tile::isOccupied()
{
	bool marked;
	m_lock.lock();
	marked = m_isOccupied; //critical section
	m_lock.unlock();
	return marked;
}

std::pair<float, float> Tile::getPosition()
{
	return std::pair<float, float>(m_x, m_y);
}

void Tile::draw(Renderer & r)
{
	m_lock.lock();
	if (!m_isOccupied)
	{
		r.drawRectOutline(Rect(m_x, m_y, m_size, m_size), m_colour);
	}
	else
	{
		r.drawRect(Rect(m_x, m_y, m_size, m_size), m_colour);
	}
	m_lock.unlock();
}

void Tile::setColour(Colour c)
{
	m_lock.lock();
	m_colour = c; //critical section
	m_lock.unlock();
}

#include "stdafx.h"
#include "Tile.h"

Tile::Tile(float x, float y, float size, bool marked) : m_x(x), m_y(y), m_marked(marked), m_size(size)
{
}

void Tile::setMarked(bool marked)
{
	m_lock.lock();
	m_marked = marked; //critical section
	m_lock.unlock();
}

bool Tile::getMarked()
{
	bool marked;
	m_lock.lock();
	marked = m_marked; //critical section
	m_lock.unlock();
	return marked;
}

std::pair<float, float> Tile::getPosition()
{
	return std::pair<float, float>(m_x, m_y);
}

void Tile::draw(Renderer & r)
{
	r.drawRectOutline(Rect(m_x, m_y, m_size, m_size), Colour(255, 255, 255, 255));
}

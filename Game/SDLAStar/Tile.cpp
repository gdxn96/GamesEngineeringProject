#include "stdafx.h"
#include "Tile.h"

Tile::Tile(std::pair<int, int> index, float x, float y, float size, bool marked) : m_x(x), m_y(y), m_isOccupied(marked), m_size(size), m_index(index), m_colour(255, 255, 255, 255), m_lock(SDL_CreateMutex())
{
}

Tile::~Tile()
{
	SDL_DestroyMutex(m_lock);
}

void Tile::isOccupied(bool marked)
{
	SDL_LockMutex(m_lock);
	m_isOccupied = marked; //critical section
	SDL_UnlockMutex(m_lock);
}

bool Tile::isOccupied()
{
	bool marked;
	SDL_LockMutex(m_lock);
	marked = m_isOccupied; //critical section
	SDL_UnlockMutex(m_lock);
	return marked;
}

std::pair<float, float> Tile::getPosition()
{
	return std::pair<float, float>(m_x, m_y);
}

void Tile::draw(Renderer & r)
{
	SDL_LockMutex(m_lock);
	if (!m_isOccupied)
	{
		if ((m_colour.r == 255 && m_colour.g == 0) || (m_colour.r == 0 && m_colour.g == 255))
		{
			r.drawRect(Rect(m_x, m_y, m_size, m_size), m_colour);
		}
		else
		{
			r.drawRectOutline(Rect(m_x, m_y, m_size, m_size), m_colour);
		}
	}
	else
	{
		r.drawRect(Rect(m_x, m_y, m_size, m_size), m_colour);
	}
	SDL_UnlockMutex(m_lock);
}

void Tile::setColour(Colour c)
{
	SDL_LockMutex(m_lock);
	m_colour = c; //critical section
	SDL_UnlockMutex(m_lock);
}

std::pair<int, int> Tile::getIndex()
{
	return m_index;
}

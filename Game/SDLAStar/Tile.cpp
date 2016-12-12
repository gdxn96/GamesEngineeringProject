#include "stdafx.h"
#include "Tile.h"

Tile::Tile(std::pair<int, int> index, float x, float y, float size, bool marked) : m_isOccupied(marked), m_size(size), m_index(index), m_lock(SDL_CreateMutex())
{
}

Tile::~Tile()
{
	SDL_DestroyMutex(m_lock);
}

void Tile::isOccupied(bool marked)
{
	m_isOccupied = marked;
}

bool Tile::isOccupied()
{
	return m_isOccupied;
}

void Tile::BeingTraversed(bool b)
{
	m_beingTraversed = b;
}

bool Tile::BeingTraversed()
{
	return m_beingTraversed;
}

std::pair<float, float> Tile::getPosition()
{
	return std::pair<float, float>( m_index.second * m_size, m_index.first * m_size);
}

void Tile::draw(Renderer & r)
{
	if (!m_isOccupied)
	{
		if (m_index.first % 2 == 0)
		{
			if (m_index.second % 2 == 1)
				r.drawRect(getRect(), Colour(102, 153, 153));
			else
				r.drawRect(getRect(), Colour(102, 153, 255));
		}
		else
		{
			if (m_index.second % 2 == 1)
				r.drawRect(getRect(), Colour(102, 153, 255));
			else
				r.drawRect(getRect(), Colour(102, 153, 153));
		}
	}
	else
	{
		r.drawRect(getRect(), Colour());
	}
}


Rect Tile::getRect()
{
	return Rect(getPosition().first, getPosition().second, m_size, m_size);
}

std::pair<int, int> Tile::getIndex()
{
	return m_index;
}

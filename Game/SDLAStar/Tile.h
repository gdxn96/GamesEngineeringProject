#pragma once
#include "Renderer.h"
#include <mutex>

class Tile
{
public:
	Tile(std::pair<int, int> index, float x, float y, float size, bool marked);
	~Tile();
	void isOccupied(bool marked);
	bool isOccupied();
	std::pair<float, float> getPosition();
	void draw(Renderer& r);
	void setColour(Colour c);
	std::pair<int, int> getIndex();
private:
	float m_x, m_y;
	std::pair<int, int> m_index;
	bool m_isOccupied;
	SDL_mutex* m_lock;
	float m_size;
	Colour m_colour;
};
#pragma once
#include <mutex>
#include "Renderer.h"

class Tile
{
public:
	Tile(float x, float y, float size, bool marked);
	void isOccupied(bool marked);
	bool isOccupied();
	std::pair<float, float> getPosition();
	void draw(Renderer& r);
	void setColour(Colour c);
private:
	float m_x, m_y;
	bool m_isOccupied;
	std::mutex m_lock;
	float m_size;
	Colour m_colour;
};
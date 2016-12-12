#pragma once
#include "Renderer.h"
#include <mutex>

class Tile
{
public:

	Tile(std::pair<int, int> index, float size, bool marked);
	~Tile();
	void isOccupied(bool marked);
	bool isOccupied();

	void BeingTraversed(bool b);
	bool BeingTraversed();
	std::pair<float, float> getPosition();
	void draw(Renderer& r);
	Rect getRect();
	std::pair<int, int> getIndex();
private:
	bool m_beingTraversed;
	std::pair<int, int> m_index;
	bool m_isOccupied;
	float m_size;
};
#pragma once
#include <mutex>
#include "Renderer.h"

class Tile
{
public:
	Tile(float x, float y, float size, bool marked);
	void setMarked(bool marked);
	bool getMarked();
	std::pair<float, float> getPosition();
	void draw(Renderer& r);
private:
	float m_x, m_y;
	bool m_marked;
	std::mutex m_lock;
	float m_size;
};
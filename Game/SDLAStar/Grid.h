#pragma once
#include <vector>
#include "Tile.h"
#include "Renderer.h"

using namespace std;

class Grid
{
public:
	Grid(int numRC, float width, float height);
	void draw(Renderer & r);
	void update(float dt);

private:
	vector<vector<Tile *>> m_grid;
};
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
	void addWalls();

private:
	vector<vector<Tile *>> m_grid;
	vector<Tile*> m_tiles;
	int m_numRowsColumns;
	int m_numWalls;
	int m_wallsTouching;
	int m_wallOffset;
};
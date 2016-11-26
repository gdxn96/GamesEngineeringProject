#pragma once
#include <vector>
#include "Tile.h"
#include "Renderer.h"
#include "Camera2D.h"
#include <iostream>
#include <iomanip>

using namespace std;

class Grid
{
public:
	Grid(int numRC, float width, float height);
	~Grid();
	void draw(Renderer & r, Camera2D * cam);
	void addWalls();

private:
	vector<vector<Tile *>> m_grid;
	vector<Tile*> m_tiles;
	float m_tileSize;
	int m_numRowsColumns;
	int m_numWalls;
	int m_wallsTouching;
	int m_wallOffset;

	int m_prevPercentage;
	void printPercentage(int percentage);
};
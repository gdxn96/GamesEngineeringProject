#pragma once
#include <vector>
#include "Tile.h"
#include "Renderer.h"
#include "Camera2D.h"
#include <iostream>
#include <iomanip>
#include <queue>
#include "Enemy.h"
#include <unordered_map>
#include "Astar.h"

using namespace std;

class Grid
{
public:
	Grid(int numRC, float width, float height);
	~Grid();
	void draw(Renderer & r, Camera2D * cam);
	void addWalls();
	void Reset();
	void SpawnEnemies();
	vector<Tile*> neighbours(Tile*) const;
	int cost(Tile* t1, Tile* t2) const;


	Tile* getTopLeft()
	{
		return m_grid[0][0];
	}

	Tile* getBottomRight()
	{
		auto & t = m_grid[m_numRowsColumns / 2 - 1][m_numRowsColumns - 1];

		return t;
	}

	void printPercentage(int percentage);

	int getSize();

private:
	vector<vector<Tile *>> m_grid;
	vector<Tile*> m_tiles;
	bool passable(int r, int c) const;
	bool inBounds(int r, int c) const;
	float m_tileSize;
	int m_numRowsColumns;
	int m_numWalls;
	int m_wallsTouching;
	int m_wallOffset;
	vector<std::pair<int, int>> dirs = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };

	int m_prevPercentage;
	
};
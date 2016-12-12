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
#include "Player.h"

using namespace std;

class Enemy;

class Grid
{
public:
	Grid(int numRC, int numEnemies, float width, float height);

	~Grid();

	void draw(Renderer & r, Camera2D * cam);

	void update(float dt);
	
	void Reset();
	
	vector<Tile*> neighbours(Tile*) const;

	int cost(Tile* t1, Tile* t2) const;

	void printPercentage(int percentage);

	int getSize();

private:
	vector<vector<Tile *>> m_grid;
	vector<Tile*> m_tiles;
	bool passable(int r, int c) const;
	bool inBounds(int r, int c) const;
	void addWalls();
	void SpawnEnemies();
	void calculateWaypoints();
	float m_tileSize;
	int m_numRowsColumns;
	int m_numWalls;
	int m_wallsTouching;
	int m_wallOffset;
	vector<std::pair<int, int>> dirs = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
	vector<Tile*> m_waypoints;
	vector<Enemy*> m_enemies;
	Player * m_player;
	vector<Tile*> m_playerPath, m_enemySpawn;
	int m_numEnemies;
	int m_prevPercentage;

	int m_playerSpawnMinX, m_playerSpawnMaxX;
	int m_enemySpawnMinX, m_enemySpawnMaxX;

	void setPlayerSpawn();
	void setEnemySpawn();
	
};
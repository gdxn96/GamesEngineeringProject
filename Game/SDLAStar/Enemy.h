#pragma once
#include "GameObject.h"
#include "BasicTypes.h"
#include "Tile.h"
#include <vector>
#include "TaskQueue.h"
#include "Grid.h"

class Grid;
class Player;

class Enemy : public GameObject 
{
public:
	enum FSMState {TILE_TRAVERSAL, CHASING_PLAYER, WAITING, WAITING_FOR_PATH, DEAD};
	Enemy(Grid* gridRef, Player* player, std::vector<Tile*> waypointsToVisit, Tile* startTile);
	void Render(Renderer& r) override;
	void Update(float dt) override;
	Rect getRect();
	void Reset();
private:
	Tile* getNextTile(Tile* previousTile);

	void traverseTile(float dt);
	Tile* getNextWaypoint(Tile* previousTile);
	Grid* m_gridRef;
	Rect m_rect;
	Tile * m_targetTile;
	Tile * m_currentTile;
	Tile * m_startTile;
	Tile * m_targetWaypoint;
	float m_timeWaiting;
	const float TIME_TO_TRAVERSE;
	FSMState m_state;
	std::vector<Tile*> m_wayPointsToVisit;
	std::vector<Tile*> m_currentTilePath;
	Player* m_player;
	Tile * m_prevPlayerTile;

	int m_taskId;
};


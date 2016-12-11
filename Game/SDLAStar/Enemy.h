#pragma once
#include "GameObject.h"
#include "BasicTypes.h"
#include "Tile.h"
#include <vector>
#include "Astar.h"
#include "TaskQueue.h"

class Enemy : public GameObject 
{
public:
	enum FSMState {TILE_TRAVERSAL, CHASING_PLAYER, WAITING, WAITING_FOR_PATH, DEAD};
	Enemy(Grid* gridRef, std::vector<Tile*> waypointsToVisit, Tile* startTile);
	void Render(Renderer& r) override;
	void Update(float dt) override;

	void Reset();
private:
	Tile* getNextTile(Tile* previousTile);
	Tile* getNextWaypoint(Tile* previousTile);
	Grid* m_gridRef;
	Rect m_rect;
	Tile * m_targetTile;
	Tile * m_currentTile;
	Tile * m_startTile;
	Tile * m_previousWaypoint;
	const float TIME_TO_TRAVERSE;
	FSMState m_state;
	std::vector<Tile*> m_wayPointsToVisit;
	std::vector<Tile*> m_currentTilePath;

	int m_taskId;
};
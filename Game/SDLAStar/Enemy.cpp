#include "stdafx.h"
#include "Enemy.h"

Enemy::Enemy(Grid * gridRef, Player* player, std::vector<Tile*> waypointsToVisit, Tile * startTile) : 
	TIME_TO_TRAVERSE(0.1f), 
	m_gridRef(gridRef),
	m_wayPointsToVisit(waypointsToVisit), 
	m_targetTile(nullptr), 
	m_currentTile(startTile), 
	m_startTile(startTile), 
	m_rect(startTile->getRect()),
	m_state(FSMState::WAITING_FOR_PATH),
	m_taskId(-1),
	m_targetWaypoint(nullptr),
	m_player(player),
	m_timeWaiting(0),
	m_prevPlayerTile(nullptr)
{
	m_targetWaypoint = getNextWaypoint(m_targetWaypoint);
}

void Enemy::Render(Renderer & r)
{
	switch (m_state)
	{
	case(DEAD):
		return;
		break;
	default:
		r.drawRect(m_rect, Colour(255, 0, 0));
		break;
	}
}

void Enemy::Update(float dt)
{
	if (m_currentTile == m_player->getCurrentTile())
	{
		m_state = DEAD;
	}

	switch (m_state)
	{
	case(TILE_TRAVERSAL):
		if (m_prevPlayerTile != nullptr && m_prevPlayerTile != m_player->getCurrentTile())
		{
			m_state = CHASING_PLAYER;
		}
		traverseTile(dt);
		break;
	case(WAITING):
		m_timeWaiting += dt;
		if (m_targetTile != nullptr && !m_targetTile->BeingTraversed() || m_timeWaiting > TIME_TO_TRAVERSE)
		{
			m_targetTile->BeingTraversed(false);
			m_state = FSMState::TILE_TRAVERSAL;
		}
		break;
	case(CHASING_PLAYER):
		m_targetWaypoint = m_player->getCurrentTile();
		m_state = WAITING_FOR_PATH;
		break;
	case(WAITING_FOR_PATH):
		if (m_taskId == -1)
		{ 
			if (m_targetWaypoint == nullptr)
			{
				m_state = CHASING_PLAYER;
				break;
			}
			m_taskId = TaskQueue::getInstance()->addJob(std::bind(AStar, m_gridRef, m_currentTile, m_targetWaypoint));
		}
		else if (TaskQueue::getInstance()->jobFinished(m_taskId))
		{
			m_currentTilePath = TaskQueue::getInstance()->getJobResults<vector<Tile*>>(m_taskId);
			m_taskId = -1;
			if (m_currentTilePath.size() != 0)
			{
				m_targetTile = nullptr;
				m_targetTile = getNextTile(m_targetTile);
				m_state = FSMState::TILE_TRAVERSAL;
			}
		}
	case(DEAD):
		m_currentTile->BeingTraversed(false);
		break;
	}
}

Rect Enemy::getRect()
{
	return m_rect;
}

void Enemy::Reset()
{
	m_currentTile = m_startTile;
	m_rect = m_currentTile->getRect();
	m_currentTilePath.clear();
	m_targetTile = nullptr;
	m_targetWaypoint = nullptr;
	m_state = FSMState::WAITING_FOR_PATH;
	m_targetWaypoint = getNextWaypoint(m_targetWaypoint);
	m_taskId = -1;
}

Tile* Enemy::getNextTile(Tile * previousTile)
{
	Tile* newTargetTile = nullptr;
	if (previousTile == nullptr)
	{
		newTargetTile = m_currentTilePath.front();
	}
	else
	{
		auto& it = std::find(m_currentTilePath.begin(), m_currentTilePath.end(), previousTile);
		if (it != m_currentTilePath.end())
		{
			it++;
			if (it != m_currentTilePath.end())
			{
				newTargetTile = *it;
			}
		}
	}
	return newTargetTile;
}

void Enemy::traverseTile(float dt)
{
	auto currentTilePos = m_currentTile->getPosition();
	auto targetTilePos = m_targetTile->getPosition();

	m_rect.pos = m_rect.pos + (Point2D(targetTilePos.first - currentTilePos.first, targetTilePos.second - currentTilePos.second) * (dt / TIME_TO_TRAVERSE));
	
	Point2D distanceVector = m_rect.pos + Point2D(-currentTilePos.first, -currentTilePos.second);
	float distFromPrevious = sqrt(distanceVector.y * distanceVector.y + distanceVector.x * distanceVector.x);
	distanceVector = Point2D(targetTilePos.first, targetTilePos.second) + Point2D(-currentTilePos.first, -currentTilePos.second);;
	float distBetweenTargetAndPrevious = sqrt(distanceVector.y * distanceVector.y + distanceVector.x * distanceVector.x);
	if (distFromPrevious >= distBetweenTargetAndPrevious)
	{
		m_currentTile = m_targetTile;
		m_rect.pos = Point2D(m_currentTile->getPosition().first, m_currentTile->getPosition().second);
		m_targetTile = getNextTile(m_targetTile);
		if (m_targetTile == nullptr)
		{
			m_currentTilePath.clear();
			m_state = FSMState::WAITING_FOR_PATH;
			m_targetWaypoint = getNextWaypoint(m_targetWaypoint);
		}
		else if (m_targetTile->BeingTraversed())
		{
			m_timeWaiting = 0;
			m_state = FSMState::WAITING;
		}
		else
		{
			m_currentTile->BeingTraversed(false);
			m_targetTile->BeingTraversed(true);
		}
	}
}

Tile * Enemy::getNextWaypoint(Tile * previousWaypoint)
{
	Tile* newTargetTile = nullptr;
	if (previousWaypoint == nullptr)
	{
		newTargetTile = m_wayPointsToVisit.front();
	}
	else
	{
		auto& it = std::find(m_wayPointsToVisit.begin(), m_wayPointsToVisit.end(), previousWaypoint);
		if (it != m_wayPointsToVisit.end())
		{
			it++;
			if (it != m_wayPointsToVisit.end())
			{
				newTargetTile = *it;
			}
		}
	}
	return newTargetTile;
}

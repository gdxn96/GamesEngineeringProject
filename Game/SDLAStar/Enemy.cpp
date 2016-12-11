#include "stdafx.h"
#include "Enemy.h"

Enemy::Enemy(Grid * gridRef, std::vector<Tile*> waypointsToVisit, Tile * startTile) : 
	TIME_TO_TRAVERSE(1.f), 
	m_gridRef(gridRef),
	m_wayPointsToVisit(waypointsToVisit), 
	m_targetTile(nullptr), 
	m_currentTile(startTile), 
	m_startTile(startTile), 
	m_rect(startTile->getRect()),
	m_state(FSMState::WAITING_FOR_PATH),
	m_taskId(-1)
{
}

void Enemy::Render(Renderer & r)
{
	switch (m_state)
	{
	case(FSMState::DEAD):
		break;
	default:
		r.drawRect(m_rect, Colour(255));
		break;
	}
}

void Enemy::Update(float dt)
{
	switch (m_state)
	{
	case(FSMState::TILE_TRAVERSAL):
		auto currentTilePos = m_currentTile->getPosition();
		auto targetTilePos = m_targetTile->getPosition();
		Point2D distanceVector = m_rect.pos + Point2D(-targetTilePos.first, -targetTilePos.second);
		float sqDistUntilTargetBefore = (distanceVector.y * distanceVector.y + distanceVector.x * distanceVector.x);
		m_rect.pos = m_rect.pos + (Point2D(targetTilePos.first - currentTilePos.first, targetTilePos.second - currentTilePos.second) * (dt / TIME_TO_TRAVERSE));
		distanceVector = m_rect.pos + Point2D(-targetTilePos.first, -targetTilePos.second);
		float sqDistUntilTargetAfter = (distanceVector.y * distanceVector.y + distanceVector.x * distanceVector.x);
		if (sqDistUntilTargetAfter > sqDistUntilTargetBefore)
		{
			m_targetTile = getNextTile(m_targetTile);
			if (m_targetTile->BeingTraversed())
			{
				m_state = FSMState::WAITING;
			}
			else
			{
				m_targetTile->BeingTraversed(true);
			}
			if (m_targetTile == nullptr)
			{
				m_currentTilePath.clear();
				m_state = FSMState::WAITING_FOR_PATH;
			}
		}
		break;
	case(FSMState::WAITING):
		if (!m_targetTile->BeingTraversed())
		{
			m_targetTile->BeingTraversed(true);
			m_state = FSMState::TILE_TRAVERSAL;
		}
		break;
	case(FSMState::CHASING_PLAYER):
		break;
	case(FSMState::WAITING_FOR_PATH):
		if (m_taskId == -1)
		{ 
			m_previousWaypoint = getNextWaypoint(m_previousWaypoint);
			m_taskId = TaskQueue::getInstance()->addJob(std::bind(AStar, m_gridRef, m_currentTile, m_previousWaypoint));
		}
		else if (TaskQueue::getInstance()->jobFinished(m_taskId))
		{
			m_currentTilePath = TaskQueue::getInstance()->getJobResults<vector<Tile*>>(m_taskId);
			m_taskId = -1;
			m_targetTile = getNextTile(m_targetTile);
			m_state = FSMState::TILE_TRAVERSAL;
		}
		break;
	}
}

void Enemy::Reset()
{
	m_currentTile = m_startTile;
	m_rect = m_currentTile->getRect();
	m_currentTilePath.clear();
	m_targetTile = nullptr;
	m_state = FSMState::WAITING_FOR_PATH;
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

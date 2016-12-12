#include "stdafx.h"
#include "Player.h"

Player::Player(std::vector<Tile*> path) : 
	m_path(path), 
	m_rect(path.front()->getRect()), 
	m_targetTile(path.front()), 
	m_currentTile(path.front()), 
	TIME_TO_TRAVERSE(2), 
	m_colour(Colour(0, 255, 0))
{
}

void Player::Update(float dt)
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
			m_targetTile = m_path.front();
		}
	}
}

Rect Player::getRect()
{
	return m_rect;
}

void Player::Render(Renderer & r)
{
	r.drawRect(m_rect, m_colour);
}

Tile * Player::getCurrentTile()
{
	return m_currentTile;
}

Tile * Player::getNextTile(Tile * previousTile)
{
	Tile* newTargetTile = nullptr;
	if (previousTile == nullptr)
	{
		newTargetTile = m_path.front();
	}
	else
	{
		auto& it = std::find(m_path.begin(), m_path.end(), previousTile);
		if (it != m_path.end())
		{
			it++;
			if (it != m_path.end())
			{
				newTargetTile = *it;
			}
		}
	}
	return newTargetTile;
}

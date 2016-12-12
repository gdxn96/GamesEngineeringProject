#pragma once
#include "GameObject.h"
#include "Tile.h"
#include <vector>
#include "BasicTypes.h"


class Player : GameObject
{
public:
	Player(std::vector<Tile*> path);
	void Update(float dt) override;
	Rect getRect();
	void Render(Renderer& r) override;
private:
	std::vector<Tile*> m_path;
	Rect m_rect;
	Colour m_colour;
	const float TIME_TO_TRAVERSE;
	Tile * m_currentTile;
	Tile* m_targetTile;

	Tile* getNextTile(Tile* previousTile);
};
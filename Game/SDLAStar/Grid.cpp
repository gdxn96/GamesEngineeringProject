#include "stdafx.h"
#include "Grid.h"

void Grid::printPercentage(int percentage)
{
	if (percentage == m_prevPercentage)
	{
		return;
	}
	for (int i = 0; i < 4; i++)
	{
		cout << "\b";
	}

	cout << setfill(' ') << setw(3) << percentage;
	cout << "%";

	m_prevPercentage = percentage;
}

int Grid::getSize()
{
	return m_numRowsColumns;
}

Grid::Grid(int numRC, float width, float height) : m_numRowsColumns(numRC)
{
	cout << "Loading Tiles...000%";
	m_tileSize = width / numRC;
	for (int i = 0; i < numRC; i++)
	{		
		m_grid.push_back(vector<Tile*>());
		for (int j = 0; j < numRC; j++)
		{
			printPercentage(int(100 * (m_tiles.size() + 1) / static_cast<float>(numRC * numRC)));
			Tile * temp = new Tile(std::pair<int, int>(i, j),j * m_tileSize, i * m_tileSize, m_tileSize, false);
			m_grid[i].push_back(temp);
			m_tiles.push_back(temp);
		} 
	}

	//meet spec for walls in tilemap
	switch (m_numRowsColumns)
	{
	case 30:
		m_numWalls = 3;
		m_wallsTouching = 1;
		break;
	case 100:
		m_numWalls = 6;
		m_wallsTouching = 2;
		break;
	case 1000:
		m_numWalls = 18;
		m_wallsTouching = 4;
		break;
	default:
		m_numWalls = 3;
		m_wallsTouching = 1;
		break;
	}
	cout << endl;

	addWalls();
	calculateWaypoints();
	SpawnEnemies();
}

Grid::~Grid()
{
	for (std::vector<Tile*>::iterator i = m_tiles.begin(); i != m_tiles.end(); i++) {
		delete *i;
	}
	m_tiles.clear();
}

void Grid::draw(Renderer & r, Camera2D* cam)
{
	//only draw tiles in viewport
	Rect camPos = (cam->getViewport() / cam->getScale());
	int maxColumn = (camPos.pos.x + camPos.size.w) / m_tileSize;
	int maxRow = (camPos.pos.y + camPos.size.h) / m_tileSize;
	for (int column = camPos.pos.x / m_tileSize; column <= maxColumn; column++) 
	{
		for (int row = camPos.pos.y / m_tileSize; row <= maxRow; row++) 
		{
			if (row < m_numRowsColumns && column < m_numRowsColumns && row >= 0 && column >= 0)
			{
				m_grid[row][column]->draw(r);
			}
		}
	}
	for (auto& enemy : m_enemies)
	{
		if (camPos.containsPoint(enemy->getRect().pos))
		{
			enemy->Render(r);
		}
	}
}

void Grid::update(float dt)
{
	for (auto& enemy : m_enemies)
	{
		enemy->Update(dt);
	}
}

void Grid::addWalls()
{
	//brute force, meet specification's map restrictions
	int edgesMet = 0;
	
	cout << "Loading Walls...000%";

	vector<int> wallColumns;
	while (m_numWalls != wallColumns.size())
	{
		int c = rand() % (m_numRowsColumns - 2) + 1; //column wall position
		auto it = std::find_if(wallColumns.begin(), wallColumns.end(), [&](int el) { return el < c + 2 && el > c - 2; }); //ensure two walls aren't stuck together
		if (it == wallColumns.end()) //if another wall is not too close
		{
			wallColumns.push_back(c);
		}
	}

	int j = 0;
	for (auto& col : wallColumns)
	{
		printPercentage(100 * static_cast<float>(j + 1) / wallColumns.size());
		for (int i = 1; i < m_numRowsColumns - 1; i++)
		{
			m_grid[i][col]->isOccupied(true);
		}
		if (edgesMet != m_wallsTouching)
		{
			if (j % 2 == 0)
			{
				m_grid[0][col]->isOccupied(true);
			}
			else
			{
				m_grid[m_numRowsColumns - 1][col]->isOccupied(true);
			}
			edgesMet++;
		}
		j++;
	}

	cout << endl;
}

void Grid::Reset()
{
	for (auto& tile : m_tiles)
	{
		tile->BeingTraversed(false);
	}
}

void Grid::SpawnEnemies()
{
	Tile * spawnTile = nullptr;
	for (int i = 0; i < 100; i++)
	{
		spawnTile = m_grid[0][0];
		m_enemies.push_back(new Enemy(this, m_waypoints, spawnTile));
	}
}

void Grid::calculateWaypoints()
{
	cout << "Calculating Waypoints" << endl;
	auto p_path = AStar(this, m_grid[0][0], m_grid[m_numRowsColumns - 1][m_numRowsColumns - 1]);
	auto path = *p_path;
	delete p_path;
	int tiles = 0;
	for (auto& tile : path)
	{
		tiles++;
		if (tiles % 10 == 1)
		{
			m_waypoints.push_back(tile); // every 10 tiles is a waypoint
		}
		if (tiles > path.size() - (path.size() / 50)) //ignore last 2 percent of tiles
		{
			break;
		}
	}
}

int Grid::cost(Tile * t1, Tile * t2) const
{
	return 1;
}

bool Grid::passable(int r, int c) const
{
	return !m_grid[r][c]->isOccupied();
}

bool Grid::inBounds(int r, int c) const
{
	return r < m_numRowsColumns && c < m_numRowsColumns && r >= 0 && c >= 0;
}

vector<Tile*> Grid::neighbours(Tile * t) const
{
	vector<Tile*> results;
	auto& i = t->getIndex();
	for (auto dir : dirs) {
		std::pair<int, int> next(i.first + dir.first, i.second + dir.second);
		if (inBounds(next.first, next.second) && passable(next.first, next.second)) {
			results.push_back(m_grid[next.first][next.second]);
		}
	}
	return results;
}

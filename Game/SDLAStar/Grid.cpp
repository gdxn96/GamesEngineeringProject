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

Grid::Grid(int numRC, float width, float height) : m_numRowsColumns(numRC)
{
	cout << "000%";
	m_tileSize = width / numRC;
	for (int i = 0; i < numRC; i++)
	{		
		m_grid.push_back(vector<Tile*>());
		for (int j = 0; j < numRC; j++)
		{
			printPercentage(int(100 * (m_tiles.size() + 1) / static_cast<float>(numRC * numRC)));
			Tile * temp = new Tile(j * m_tileSize, i * m_tileSize, m_tileSize, false);
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
}

Grid::~Grid()
{
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
}

void Grid::addWalls()
{
	//brute force, meet specification's map restrictions
	int column = 0;
	int row = 0;
	int edgesMet = 0;
	int i = 0;
	cout << "000%";

	for (auto& tile : m_tiles)
	{
		i++;
		printPercentage(100 * static_cast<float>(i + 1) /m_tiles.size());
		column++;
		if (column % ((m_numRowsColumns / m_numWalls)) == (m_numRowsColumns / m_numWalls) / 2)
		{
			if (row != m_numRowsColumns - 1)
			{
				if (row != 0)
				{
					tile->isOccupied(true);
				}
				else if (m_wallsTouching != edgesMet && column % 3 == 0)
				{
					tile->isOccupied(true);
					edgesMet++;
				}
			}
		}
		if (column == m_numRowsColumns)
		{
			column = 0;
			row++;
		}
	}

	cout << endl;
}

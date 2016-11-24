#include "stdafx.h"
#include "Grid.h"

Grid::Grid(int numRC, float width, float height) : m_numRowsColumns(numRC)
{
	float offset = width / numRC;
	for (int i = 0; i < numRC; i++)
	{
		m_grid.push_back(vector<Tile*>());
		for (int j = 0; j < numRC; j++)
		{
			Tile * temp = new Tile(j * offset, i * offset, offset, false);
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
}

void Grid::draw(Renderer & r)
{
	for (auto& row : m_grid)
	{
		for (auto& tile : row)
		{
			tile->draw(r);
		}
	}
}

void Grid::addWalls()
{
	//brute force, meet specification's map restrictions
	int column = 0;
	int row = 0;
	int edgesMet = 0;

	for (auto& tile : m_tiles)
	{
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
}

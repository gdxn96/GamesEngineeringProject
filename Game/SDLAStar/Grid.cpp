#include "stdafx.h"
#include "Grid.h"

Grid::Grid(int numRC, float width, float height)
{
	float offset = width / numRC;
	for (int i = 0; i < numRC; i++)
	{
		m_grid.push_back(vector<Tile*>());
		for (int j = 0; j < numRC; j++)
		{
			m_grid[i].push_back(new Tile(i * offset, j * offset, offset, false));
		}
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

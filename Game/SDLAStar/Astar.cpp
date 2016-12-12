#include "stdafx.h"
#include "Astar.h"

vector<Tile*> reconstruct_path(Tile * start, Tile * goal, unordered_map<Tile*, Tile*>& cameFrom)
{
	vector<Tile*> path;
	Tile* current = goal;
	path.push_back(current);
	while (!(current == start))
	{
		current = cameFrom[current];
		path.push_back(current);
	}
	//path.push_back(start); // optional
	std::reverse(path.begin(), path.end());
	return path;
}

vector<Tile*>* AStar(const Grid * graph, Tile * start, Tile * goal)
{
	unordered_map<Tile*, Tile*> cameFrom = unordered_map<Tile*, Tile*>();
	unordered_map<Tile*, int> gCostUntil = unordered_map<Tile*, int>();
	PriorityQueue<Tile*, float> open;
	unordered_map<Tile*, bool> closed;
	vector<Tile*>* path = nullptr;
	open.put(start, 0);

	cameFrom[start] = start;
	gCostUntil[start] = 0;

	while (!open.empty())
	{
		auto current = open.get();
		if (closed[current])
		{
			continue;
		}

		if (current == goal)
		{
			path =  new vector<Tile*>(reconstruct_path(start, goal, cameFrom));
			break;
		}
		vector<Tile*> neighbours = graph->neighbours(current);
		for (auto next : neighbours)
		{
			if (closed[next])
			{
				continue;
			}
			int newGCost = gCostUntil[current] + graph->cost(current, next);

			//if its never been checked OR we've found a lower cost way of reaching tile
			if (!gCostUntil.count(next) || newGCost < gCostUntil[next])
			{
				gCostUntil[next] = newGCost;
				int priority = newGCost + heuristic(next, goal);
				open.put(next, priority);
				cameFrom[next] = current;
			}
		}
		closed[current] = true;
	}

	//unsolvable path if nullptr, should never happen on this map
	return path;
}

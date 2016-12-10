#pragma once
#include "BasicTypes.h"
#include <vector>
#include "Grid.h"
#include <queue>
#include <unordered_map>

using namespace std;

inline int heuristic(Tile* a, Tile* b) 
{
	return 2 * (abs(a->getIndex().first - b->getIndex().first) + abs(a->getIndex().second - b->getIndex().second));
}

template<typename T, typename priorityT>
struct PriorityQueue {
	typedef pair<priorityT, T> PQElement;
	vector<PQElement> elements;

	inline bool empty() const { return elements.empty(); }

	inline void put(T item, priorityT priority)
	{
		elements.push_back(PQElement(priority, item));
		std::sort(elements.begin(), elements.end(), [&](PQElement e1, PQElement e2) { return e2.first < e1.first; });
	}

	inline T get() {
		PQElement bestItem = elements.back();
		elements.pop_back();
		return bestItem.second;
	}
};

vector<Tile*> reconstruct_path(
	Tile* start,
	Tile* goal,
	unordered_map<Tile*, Tile*>& cameFrom
) {
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

vector<Tile*>* AStar(const Grid * graph,
		   Tile* start,
		   Tile* goal,
		   unordered_map<Tile*, Tile*>& cameFrom,
		   unordered_map<Tile*, int>& gCostUntil)
{
	PriorityQueue<Tile*, float> open;
	unordered_map<Tile*, bool> closed;
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
		current->setColour(Colour(0, 255, 0, 255));

		if (current == goal) 
		{
			return new vector<Tile*>(reconstruct_path(start, goal, cameFrom));
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

	return nullptr;
}
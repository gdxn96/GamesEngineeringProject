#pragma once
#include "BasicTypes.h"
#include <vector>
#include "Grid.h"
#include <queue>
#include <unordered_map>

class Grid;

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

vector<Tile*> reconstruct_path(Tile* start, Tile* goal, unordered_map<Tile*, Tile*>& cameFrom);
vector<Tile*>* AStar(const Grid * graph, Tile* start, Tile* goal);
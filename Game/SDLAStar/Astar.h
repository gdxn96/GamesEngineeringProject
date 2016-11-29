#pragma once
#include "BasicTypes.h"
#include <vector>
#include "Grid.h"
#include <queue>
#include <unordered_map>

using namespace std;

inline int heuristic(Tile* a, Tile* b) 
{
	return (abs(b->getIndex().first - a->getIndex().first) + abs(b->getIndex().second - a->getIndex().second));
}

template<typename T, typename priority_t>
struct PriorityQueue {
	typedef pair<priority_t, T> PQElement;
	vector<PQElement> elements;

	inline bool empty() const { return elements.empty(); }

	inline void put(T item, priority_t priority) {
		auto& element = std::find_if(elements.begin(), elements.end(),
			[&](std::pair<float, Tile*>& element) { return element.second == item; });
		if (element != elements.end())
		{
			(*element).first = priority;
		}
		else
		{
			elements.push_back(PQElement(priority, item));
		}

		std::sort(elements.begin(), elements.end(), [&](std::pair<float, Tile*>& e1, std::pair<float, Tile*>& e2) { return e2.first < e1.first; });
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
	unordered_map<Tile*, Tile*>& came_from
) {
	vector<Tile*> path;
	Tile* current = goal;
	path.push_back(current);
	while (!(current == start)) 
	{
		current = came_from[current];
		path.push_back(current);
	}
	//path.push_back(start); // optional
	std::reverse(path.begin(), path.end());
	return path;
}

void AStar(const Grid& graph,
		   Tile* start,
		   Tile* goal,
		   unordered_map<Tile*, Tile*>& came_from,
		   unordered_map<Tile*, float>& cost_so_far)
{
	PriorityQueue<Tile*, float> open;
	std::unordered_map<Tile*, bool> closed;
	open.put(start, 0);

	came_from[start] = start;
	cost_so_far[start] = 0;

	while (!open.empty())
	{
		auto current = open.get();
		closed[current] = true;
		current->setColour(Colour(0, 255, 0, 255));

		if (current == goal) 
		{
			break;
		}
		vector<Tile*> neighbours = graph.neighbours(current);
		for (auto next : neighbours)
		{
			if (closed[next])
			{
				continue;
			}
			double new_cost = cost_so_far[current] + graph.cost(current, next);
			if (!cost_so_far.count(next) || new_cost < cost_so_far[next]) 
			{
				cost_so_far[next] = new_cost;
				double priority = new_cost + heuristic(next, goal);
				open.put(next, priority);
				came_from[next] = current;
			}
		}
	}
}
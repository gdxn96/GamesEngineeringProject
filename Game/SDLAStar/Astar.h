#pragma once
#include "BasicTypes.h"
#include <vector>
#include "Grid.h"
#include <queue>
#include <unordered_map>

using namespace std;

inline float heuristic(Tile* a, Tile* b) 
{
	return abs(b->getPosition().first - a->getPosition().first) + abs(b->getPosition().first - a->getPosition().second);
}

template<typename T, typename priority_t>
struct PriorityQueue {
	typedef pair<priority_t, T> PQElement;
	priority_queue<PQElement, vector<PQElement>,
		std::greater<PQElement>> elements;

	inline bool empty() const { return elements.empty(); }

	inline void put(T item, priority_t priority) {
		elements.emplace(priority, item);
	}

	inline T get() {
		T best_item = elements.top().second;
		elements.pop();
		return best_item;
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
	PriorityQueue<Tile*, float> frontier;
	frontier.put(start, 0);

	came_from[start] = start;
	cost_so_far[start] = 0;

	while (!frontier.empty()) 
	{
		auto current = frontier.get();

		if (current == goal) 
		{
			break;
		}
		vector<Tile*> neighbours = graph.neighbours(current);
		for (auto next : neighbours)
		{
			double new_cost = cost_so_far[current] + graph.cost(current, next);
			if (!cost_so_far.count(next) || new_cost < cost_so_far[next]) 
			{
				cost_so_far[next] = new_cost;
				double priority = new_cost + heuristic(next, goal);
				frontier.put(next, priority);
				came_from[next] = current;
			}
		}
	}
}
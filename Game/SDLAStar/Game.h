#pragma once

#include <vector>

#include "Renderer.h"
#include "GameObject.h"
#include "InputManager.h"
#include "EventListener.h"
#include "Grid.h"
#include "Camera2D.h"

#include "TaskQueue.h"


/** The game objct whic manages the game loop*/
class Game:public EventListener
{
	InputManager inputManager;
	Renderer renderer;

	std::vector<GameObject*> gameObjects;

	unsigned int lastTime;//time of last update;

	bool pause;
	bool quit;

	Grid * m_grid;
	Grid * m_grid1;
	Grid * m_grid2;
	Grid * m_grid3;
	Camera2D * m_camera;
	Size2D m_screenSize;
	Size2D m_worldSize;
	int m_prevGrid;
	int m_jobId;

	void resetWorld(int numNPCs, Grid* grid, float scale);
public:
	Game(Size2D screenSize, Size2D worldSize);
	~Game();

	bool init();
	void destroy();

	void update();
	void render();
	void loop();

	void onEvent(EventListener::Event);
};


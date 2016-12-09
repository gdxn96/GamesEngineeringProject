#include "stdafx.h"

#include <iostream>
using namespace std;

#include "LTimer.h"
#include "Game.h"

#include "Astar.h"


const int SCREEN_FPS = 100;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;
void Game::resetWorld(int numNPCs, int gridSize, float scale)
{
	cout << "Resetting World..." << endl;
	m_grid = Grid(gridSize, m_worldSize.w, m_worldSize.h);
	m_grid.addWalls();
	delete m_camera;
	m_camera = new Camera2D(Rect(0, 0, m_screenSize.w, m_screenSize.h), scale);
	m_camera->setLevelSize(Size2D(m_worldSize.w, m_worldSize.h));
	renderer.setNewCamera(m_camera);
	cout << gridSize << "x" << gridSize << " World loaded" << endl << endl;

	/*const Grid gridCopy = m_grid;
	std::unordered_map<Tile*, Tile*> cameFrom = unordered_map<Tile*, Tile*>();
	std::unordered_map<Tile*, int> costSoFar = unordered_map<Tile*, int>();
	TaskQueue::getInstance()->addJob(std::bind(AStar, gridCopy, m_grid.getTopLeft(), m_grid.getBottomRight(), cameFrom, costSoFar));*/
}

Game::Game(Size2D screenSize, Size2D worldSize) : m_grid(Grid(30, worldSize.w, worldSize.h)), m_prevGrid(0), m_camera(new Camera2D(Rect(0, 0, screenSize.w, screenSize.h), 1)), m_screenSize(screenSize), m_worldSize(worldSize)
{
	TaskQueue::getInstance()->spawnWorkers();
	quit = false;
	resetWorld(0, 30, 1);
	m_camera->setLevelSize(Size2D(worldSize.w, worldSize.h));
}


Game::~Game()
{
}


bool Game::init() {	
	Size2D winSize(m_screenSize.w, m_screenSize.h);
	srand(0);

	//creates our renderer, which looks after drawing and the window
	renderer.init(winSize,"A* Threading", m_camera);


	//create some game objects


	//add some game objects
	
	lastTime = LTimer::gameTime();

	//inputManager.AddListener(EventListener::Event::EXAMPLE, EventListener);

	inputManager.AddListener(EventListener::Event::QUIT, this);
	inputManager.AddListener(EventListener::Event::UP, this);
	inputManager.AddListener(EventListener::Event::DOWN, this);
	inputManager.AddListener(EventListener::Event::LEFT, this);
	inputManager.AddListener(EventListener::Event::RIGHT, this);
	inputManager.AddListener(EventListener::Event::ZOOM_IN, this);
	inputManager.AddListener(EventListener::Event::ZOOM_OUT, this);
	inputManager.AddListener(EventListener::Event::CHANGE_GRID, this);
	return true;

}


void Game::destroy()
{
	//empty out the game object vector before quitting
	for (std::vector<GameObject*>::iterator i = gameObjects.begin(); i != gameObjects.end(); i++) {
		delete *i;
	}
	gameObjects.clear();
	renderer.destroy();
}

//** calls update on all game entities*/
void Game::update()
{
	unsigned int currentTime = LTimer::gameTime();//millis since game started
	float deltaTime = (currentTime - lastTime) / 1000.0;//time since last update

	//call update on all game objects
	for (std::vector<GameObject*>::iterator i = gameObjects.begin(); i != gameObjects.end(); i++) {
		(*i)->Update(deltaTime);
	}

	//save the curent time for next frame
	lastTime = currentTime;
}

//** calls render on all game entities*/

void Game::render()
{
	renderer.clear(Colour(0,0,0));// prepare for new frame
	
	//render every object
	for (std::vector<GameObject*>::iterator i = gameObjects.begin(), e= gameObjects.end(); i != e; i++) {
		(*i)->Render(renderer);
	}
	m_grid.draw(renderer, m_camera);

	renderer.present();// display the new frame (swap buffers)
}

/** update and render game entities*/
void Game::loop()
{
	LTimer capTimer;//to cap framerate

	int frameNum = 0;
	while (!quit) { //game loop
		capTimer.start();

		inputManager.ProcessInput();

		update();
		render();

		int frameTicks = capTimer.getTicks();//time since start of frame
		if (frameTicks < SCREEN_TICKS_PER_FRAME)
		{
			//Wait remaining time before going to next frame
			SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
		}
	}
}

void Game::onEvent(EventListener::Event evt) {
	switch (evt)
	{
	case(EventListener::Event::QUIT):
		quit = true;
		break;
	case(EventListener::Event::LEFT):
		m_camera->MoveLeft();
		break;
	case(EventListener::Event::RIGHT):
		m_camera->MoveRight();
		break;
	case(EventListener::Event::UP):
		m_camera->MoveUp();
		break;
	case(EventListener::Event::DOWN):
		m_camera->MoveDown();
		break;
	case(EventListener::Event::ZOOM_IN):
		m_camera->increaseScale();
		break;
	case(EventListener::Event::ZOOM_OUT):
		m_camera->decreaseScale();
		break;
	case(EventListener::Event::CHANGE_GRID):
		switch (m_prevGrid)
		{
		case(0):
			resetWorld(0, 100, 3);
			m_prevGrid = 1;
			break;
		case(1):
			resetWorld(0, 1000, 30);
			m_prevGrid = 2;
			break;
		case(2):
			resetWorld(0, 30, 1);
			m_prevGrid = 0;
			break;
		}
		break;
	default:
		break;
	}

}

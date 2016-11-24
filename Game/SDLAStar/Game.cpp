#include "stdafx.h"

#include <iostream>
using namespace std;



#include "LTimer.h"
#include "Game.h"

const int SCREEN_FPS = 100;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;


Game::Game() : m_grid(Grid(30, 1080, 1080))
{
	quit = false;
	m_grid.addWalls();
}


Game::~Game()
{
}


bool Game::init() {	
	Size2D winSize(1080, 1080);
	srand(0);

	//creates our renderer, which looks after drawing and the window
	renderer.init(winSize,"A* Threading");


	//create some game objects


	//add some game objects
	
	lastTime = LTimer::gameTime();

	//inputManager.AddListener(EventListener::Event::EXAMPLE, EventListener);

	inputManager.AddListener(EventListener::Event::QUIT, this);

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
	m_grid.draw(renderer);

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

		if(!pause) //in pause mode don't bother updateing
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
		
	if (evt == EventListener::Event::QUIT) {
		quit=true;
	}

}

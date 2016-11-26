#include "stdafx.h"
#include <iostream>
using namespace std;
#define SDL_MAIN_HANDLED

#ifdef __APPLE__
    #include "SDL2/SDL.h"
#elif defined(_WIN64) || defined(_WIN32)
    #include "SDL.h"
#endif

#include "Renderer.h"




Renderer::Renderer():sdl_renderer(NULL)
{
	
}

bool Renderer::init(const Size2D& winSize,const char* title, Camera2D* cam) {

	m_camera = cam;
	int e=SDL_Init(SDL_INIT_EVERYTHING);              // Initialize SDL2
	windowSize = winSize;
	if (e != 0) {
		// problem with SDL?...
		cout << "Could not init SDL: " << SDL_GetError() << std::endl;
		return false;
	}

	// Create an application window with the following settings:
	window = SDL_CreateWindow(
		title,							   // window title
		SDL_WINDOWPOS_UNDEFINED,           // initial x position
		SDL_WINDOWPOS_UNDEFINED,           // initial y position
		(int)winSize.w,                    // width, in pixels
		(int)winSize.h,                    // height, in pixels
		SDL_WINDOW_OPENGL                  // flags - see below
	);

	// Check that the window was successfully created
	if (window == NULL) {
		// In the case that the window could not be made...
		cout << "Could not create window: " << SDL_GetError() << std::endl;
		return false;
	}

	sdl_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (sdl_renderer == NULL) {
		// In the case that the renderer could not be made...
		cout << "Could not create renderer: " << SDL_GetError() << std::endl;
		return false;
	}
	
	return true;
}

void Renderer::setNewCamera(Camera2D * newCam)
{
	m_camera = newCam;
}

void Renderer::drawRectOutline(const Rect& r, const Colour& c)
{
	Rect tRect = cameraTransform(r);
	SDL_SetRenderDrawColor(sdl_renderer, c.r, c.g, c.b, c.a);
	SDL_Rect sr;
	sr.h = tRect.size.h;
	sr.w = tRect.size.w;
	sr.x = tRect.pos.x;
	sr.y = tRect.pos.y;
	SDL_RenderDrawRect(sdl_renderer, &sr);
}

void Renderer::drawRect(const Rect& r, const Colour& c) {
	Rect tRect = cameraTransform(r);
	SDL_SetRenderDrawColor(sdl_renderer, c.r, c.g, c.b, c.a);
	SDL_Rect sr;
	sr.h = tRect.size.h;
	sr.w = tRect.size.w;
	sr.x = tRect.pos.x;
	sr.y = tRect.pos.y;
	SDL_RenderFillRect(sdl_renderer, &sr);

}

void Renderer::present() { //swap buffers
	SDL_RenderPresent(sdl_renderer);
}

void Renderer::clear(const Colour& col) {
	SDL_SetRenderDrawColor(sdl_renderer, col.r, col.g, col.b, col.a);
	SDL_RenderClear(sdl_renderer);

}

Rect Renderer::cameraTransform(Rect r)
{
	r = r * m_camera->getScale();
	r.pos.x -= m_camera->getViewport().pos.x;
	r.pos.y -= m_camera->getViewport().pos.y;
	return r;
}

/**Destroys SDL_Window and SDL_Renderer*/
void Renderer::destroy() {
	SDL_DestroyRenderer(sdl_renderer);
	SDL_DestroyWindow(window);
}

Renderer::~Renderer()
{
}

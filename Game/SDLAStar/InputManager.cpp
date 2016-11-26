#include "stdafx.h"
#include "InputManager.h"


InputManager::InputManager()
{
}


InputManager::~InputManager()
{
}

void InputManager::AddListener(EventListener::Event evt,EventListener *listener)
{
	if (listeners.find(evt) == listeners.end()) { //event not in map add it.
		listeners[evt] = new std::vector<EventListener*>();
	}

	listeners[evt]->push_back(listener);

}

//Send the events to interested objects
void InputManager::Dispatch(EventListener::Event evt)
{

	if (listeners.find(evt) != listeners.end()) {
		
		for (auto const &listener : *listeners[evt]) { //go through all listeners for this event
			listener->onEvent(evt); //Call on event for the listener

		}
	}
	
}


//Gnereate events
void InputManager::ProcessInput()
{
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
		switch (e.type) {
			/* Keyboard event */
			case SDL_KEYDOWN:
				switch (e.key.keysym.sym) {
				case SDLK_ESCAPE:
					Dispatch(EventListener::Event::QUIT);
					break;
				case SDLK_w:
					Dispatch(EventListener::Event::UP);
					break;
				case SDLK_a:
					Dispatch(EventListener::Event::LEFT);
					break;
				case SDLK_s:
					Dispatch(EventListener::Event::DOWN);
					break;
				case SDLK_d:
					Dispatch(EventListener::Event::RIGHT);
					break;
				case SDLK_UP:
					Dispatch(EventListener::Event::ZOOM_IN);
					break;
				case SDLK_DOWN:
					Dispatch(EventListener::Event::ZOOM_OUT);
					break;

				}
				break;

			
			/* SDL_QUIT event (window close) */
			case SDL_QUIT:
				Dispatch(EventListener::Event::QUIT);
				break;

			default:
				break;
			}
		
		
	
	}
	//check for exit
}

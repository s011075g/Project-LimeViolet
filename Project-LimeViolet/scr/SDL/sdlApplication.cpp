#include "SDLApplication.h"
#include <SDL/SDL.h>
#include "../IO/EntityReader.h"

SDLApplication* SDLApplication::Create()
{
	const uint32_t flags = SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS;
	uint32_t initalized = SDL_WasInit(flags);
	if(initalized != flags && SDL_Init(flags) != 0)
	{ 
		Utilities::Write(SDL_GetError(), Utilities::EXTREME_LEVEL);
		return nullptr;
	}
	return new SDLApplication();
}

SDLApplication::~SDLApplication()
{
	numInstances--;
	if (numInstances == 0)
		SDL_Quit();
}

void SDLApplication::ProcessMessages(double delta, IApplicationEventHandler& eventHandler)
{
	SDL_Event event;
	(void)delta;

	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
		case SDL_KEYDOWN:
			eventHandler.OnKeyDown(event.key.keysym.scancode, event.key.repeat != 0);
			break;
		case SDL_KEYUP:
			eventHandler.OnKeyUp(event.key.keysym.scancode, event.key.repeat != 0);
			break;
		case SDL_MOUSEBUTTONDOWN:
			eventHandler.OnMouseDown(event.button.button, event.button.clicks);
			break;
		case SDL_MOUSEBUTTONUP:
			eventHandler.OnMouseUp(event.button.button, event.button.clicks);
			break;
		case SDL_MOUSEMOTION:
			eventHandler.OnMouseMove(event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel);
			break;
		case SDL_QUIT:
			_isAppRunning = false;
			break;
		default:
			break;
		}
	}
}

bool SDLApplication::IsRunning() const
{
	return _isAppRunning;
}

SDLApplication::SDLApplication()
	: _isAppRunning(true)
{
	numInstances++;
}

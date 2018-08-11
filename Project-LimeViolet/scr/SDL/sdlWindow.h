#pragma once
#include "SDLApplication.h"
#include <string>
#include <SDL/SDL.h>

class SDLWindow
{
public:
	SDLWindow(const SDLApplication& app, int32_t width, int32_t height, const std::string title);
	virtual ~SDLWindow();

	void ChangeSize(int32_t width, int32_t height);
	void Present();

	SDL_Window* GetWindowHandle() const;
	int32_t GetWidth() const;
	int32_t GetHeight() const;

private:
	SDL_Window* _window;
	int32_t _width;
	int32_t _height;
};


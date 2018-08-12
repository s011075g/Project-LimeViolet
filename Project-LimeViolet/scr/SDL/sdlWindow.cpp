#include "sdlWindow.h"

SDLWindow::SDLWindow(const SDLApplication& app, int32_t width, int32_t height, const std::string title)
	: _width(width), _height(height)
{
	_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_VULKAN);
}

SDLWindow::~SDLWindow()
{
	SDL_DestroyWindow(_window);
}

void SDLWindow::ChangeTitle(const std::string title) const
{
	SDL_SetWindowTitle(_window, title.c_str());
}

void SDLWindow::ChangeSize(int32_t width, int32_t height)
{
	SDL_SetWindowSize(_window, width, height);
	_width = width;
	_height = height;
}

void SDLWindow::Present()
{
	//todo
}

std::string SDLWindow::GetTitle() const
{
	return SDL_GetWindowTitle(_window);
}

SDL_Window* SDLWindow::GetWindowHandle() const
{
	return _window;
}

int32_t SDLWindow::GetWidth() const
{
	return _width;
}

int32_t SDLWindow::GetHeight() const
{
	return _height;
}

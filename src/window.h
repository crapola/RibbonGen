#ifndef WINDOW_H
#define WINDOW_H
#include <SDL2/SDL.h>
#include "displaysettings.hpp"

// SDL window and initialization.

class Window
{
public:
	Window(const DisplaySettings& settings);
	~Window();
	Window(const Window&)=delete;
	Window& operator=(const Window&)=delete;

	SDL_Point Size() const;
	SDL_Surface* Surface() const;
	SDL_Window* operator()() const;
private:
	SDL_Window* _window;
};

#endif // WINDOW_H

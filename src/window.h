#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include "displaysettings.hpp"

// SDL initialization and main window.

class Window
{
public:
	Window(const DisplaySettings& settings);
	~Window();
	Window(const Window&)=delete;
	Window& operator=(const Window&)=delete;

	SDL_GLContext GetContext() const;
	SDL_Window* GetWindow() const;
private:
	SDL_Window* _window;
	SDL_GLContext _context; // void*
};

#endif // WINDOW_H

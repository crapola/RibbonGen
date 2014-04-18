#ifndef CONTEXT_H
#define CONTEXT_H
#include <SDL2/SDL.h>

// OpenGL context.

class Context
{
public:
	Context(SDL_Window*);
	~Context();
	Context(const Context&)=delete;
	Context& operator=(const Context&)=delete;
private:
	SDL_GLContext _context; // void*
};

#endif // CONTEXT_H

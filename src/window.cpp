#include "window.h"
#include <GL/glew.h>
#include <iostream>
#include <stdexcept>

Window::Window(const DisplaySettings& p_s):_window(nullptr),_context(nullptr)
{
	if (SDL_Init(SDL_INIT_VIDEO)<0)
	{
		std::string s("Unable to init SDL: ");
		s+=SDL_GetError();
		throw std::runtime_error(s);
	}
	// Hints
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE,	5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,	5);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,	5);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,	5);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
	SDL_Window* screen = SDL_CreateWindow(p_s.title.c_str(),
										  SDL_WINDOWPOS_CENTERED,
										  SDL_WINDOWPOS_CENTERED,
										  p_s.width,
										  p_s.height,
										  SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
	if (screen==nullptr)
	{
		std::string s("Unable to create window: ");
		s+=SDL_GetError();
		throw std::runtime_error(s);
	}
	_context=SDL_GL_CreateContext(screen);
	GLenum err=glewInit();
	if (GLEW_OK!=err)
	{
		std::string s("Glew init failed: ");
		const char* c=reinterpret_cast<const char*>(glewGetErrorString(err));
		s+=std::string(c);
		throw std::runtime_error(s);
	}
	std::cout<<"Using GLEW "<<glewGetString(GLEW_VERSION)<<'\n';
	std::cout<<"OpenGL version "<<glGetString(GL_VERSION)<<'\n';
	_window=screen;
}

Window::~Window()
{
	SDL_GL_DeleteContext(_context);
	SDL_DestroyWindow(_window);
	SDL_Quit();
}

SDL_GLContext Window::GetContext() const
{
	return _context;
}

SDL_Window* Window::GetWindow() const
{
	return _window;
}

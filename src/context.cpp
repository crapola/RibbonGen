#include "context.h"
#include <GL/glew.h>
#include <iostream>
#include <stdexcept>
#include <string>


Context::Context(SDL_Window* p_screen):_context(nullptr)
{
	_context=SDL_GL_CreateContext(p_screen);
	if (_context==nullptr)
	{
		throw std::runtime_error("SDL_GL_CreateContext returned null.");
	}
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
}

Context::~Context()
{
	SDL_GL_DeleteContext(_context);
}
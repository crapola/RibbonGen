#include <iostream>
#include <GL/glew.h>
#include <SDL2/SDL.h>

bool LoadTextureBMP(const char* p_fileName)
{
	// 24 bits=BGR.
	SDL_Surface* surface=SDL_LoadBMP(p_fileName);
	if (surface==NULL)
	{
		std::cerr<<"Error loading "<<p_fileName<<".\n";
		return false;
	}
	GLenum texture_format=GL_RGB;
	GLint  nOfColors;

	if (!1&(surface->w-1))
	{
		std::cout<<p_fileName<<"'s width is not a power of 2.\n";
	}

	if (!1&(surface->h-1))
	{
		std::cout<<p_fileName<<"'s height is not a power of 2.\n";
	}

	// Get the number of channels in the SDL surface
	nOfColors=surface->format->BytesPerPixel;
	if (nOfColors==4)     // contains an alpha channel
	{
		if (surface->format->Rmask==0x000000FF)
			texture_format=GL_RGBA;
		else
			texture_format=GL_BGRA;
	}
	else if (nOfColors==3)       // no alpha channel
	{
		if (surface->format->Rmask==0x000000FF)
			texture_format=GL_RGB;
		else
			texture_format=GL_BGR;
	}
	else
	{
		std::cout<<"warning: the image is not truecolor\n";
	}

	GLint currentlyBound;
	glGetIntegerv(GL_TEXTURE_BINDING_2D,&currentlyBound);
	std::cout<<"GL_TEXTURE_BINDING_2D is "<<currentlyBound<<".\n";
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, nOfColors, surface->w, surface->h, 0,
				 texture_format, GL_UNSIGNED_BYTE, surface->pixels);

	SDL_FreeSurface(surface);
	return true;
}
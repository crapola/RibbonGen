#include <fstream>
#include <stdexcept>
#include <iostream>
#include "app.h"
#include "context.h"
#include "genloop.hpp"
#include "window.h"

int main(int,char**)
{
	/*	std::streambuf *coutbuf = std::cout.rdbuf();
		std::streambuf *cerrbuf = std::cerr.rdbuf();
		std::ofstream out("log.txt");
		std::cout.rdbuf(out.rdbuf());
		std::cerr.rdbuf(out.rdbuf());*/
	int returnCode=EXIT_SUCCESS;
	try
	{
		// Hints
		/*SDL_GL_SetAttribute(SDL_GL_RED_SIZE,	5);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,	5);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,	5);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,	5);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);*/
		DisplaySettings settings {800,600,"RibbonGen"};
		Window wind("RibbonGen",800,600,SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
		SDL_Window* w=wind();
		Context context(w);
		App app(w,settings);
		auto swapper=[w]()
		{
			SDL_GL_SwapWindow(w);
		};
		Loopy<App>(app,swapper);
	}
	catch (std::runtime_error e)
	{
		std::cerr<<"Runtime error!\n"<<e.what()<<std::endl;
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Error",e.what(),0);// bug
		returnCode=EXIT_FAILURE;
	}
	std::cout<<"The program exited normally ("<<returnCode<<")."<<std::endl;
	/*	std::cout.rdbuf(coutbuf);
		std::cerr.rdbuf(cerrbuf);*/
	return returnCode;
}

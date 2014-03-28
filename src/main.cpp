#include <fstream>
#include <stdexcept>
#include <iostream>
#include "app.h"
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
		DisplaySettings settings {800,600,"RibbonGen"};
		Window wind(settings);
		App app(wind.GetWindow(),settings);
		SDL_Window* w=wind.GetWindow();
		auto swapper=[w](){SDL_GL_SwapWindow(w);};
		Loopy<App>(app,swapper);
	}
	catch (std::runtime_error e)
	{
		std::cerr<<"Runtime error!\n"<<e.what()<<std::endl;
		returnCode=EXIT_FAILURE;
	}
	std::cout<<"The program exited normally ("<<returnCode<<")."<<std::endl;
/*	std::cout.rdbuf(coutbuf);
	std::cerr.rdbuf(cerrbuf);*/
	return returnCode;
}

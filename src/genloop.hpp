#ifndef GENLOOP_HPP_INCLUDED
#define GENLOOP_HPP_INCLUDED

#include <SDL2/SDL.h>

#include "chrono.h"

// A implements update, redner..etcetc
template <class A,class T_swapper>
void Loopy(A& a,T_swapper& swapper)
{
	Chrono GCHRONO;
	Sint32 ticksPerFrame=15;
	Sint32 acc=0;
	Uint32 ticksNow=0,ticksPrev=SDL_GetTicks(),delta=0;
	bool ok=true;
	SDL_Event event;
	while (ok)
	{
		ticksNow=SDL_GetTicks();
		delta=ticksNow-ticksPrev;
		if (delta>1000*4) // Cap at 4s per frame
		{
			delta=1000*4;
		}
		acc+=delta;
		ticksPrev=ticksNow;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
					ok=false;
					break;
				case SDL_KEYDOWN:
					if (event.key.keysym.sym==SDLK_ESCAPE)
						ok=false;
					break;
				default:
					break;
			}
			a.ProcessEvent(event);
		}
		if (acc>=ticksPerFrame)
		{
			GCHRONO.Start();
			while (acc>=ticksPerFrame)
			{
				if (!a.Update()) ok=false;
				acc-=ticksPerFrame;
				SDL_Delay(0);
			}
			a.Render();
			GCHRONO.Stop();
			swapper();
		}
		else
			SDL_Delay(0);
	}
}
#endif // GENLOOP_HPP_INCLUDED

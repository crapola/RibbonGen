#include "app.h"
#include "checkglerrors.h"
#include "debuglog.hpp"
#include <iostream>
#include <GL/glew.h>
#include <SDL2/SDL.h>

App::App(SDL_Window* p_surf,DisplaySettings p_setts):_screen(p_surf),
	_displaySettings(p_setts),
	_timer(0.0f),_gui(),_ribbon(),_uistate(S_BASE)
{
	Init();
}

App::~App()
{
	std::cout<<_timer<<" seconds.\n";
}

//------------------------------------------------------------------------------

void App::ProcessEvent(const SDL_Event& p_event)
{
	int rmx,rmy;
	Uint32 ms=SDL_GetRelativeMouseState(&rmx,&rmy);
	switch (p_event.type)
	{
		default:
			break;
		case SDL_MOUSEBUTTONUP:
			_ribbon.Dampen(0.9375f); // Let it roll.
			break;
		case SDL_MOUSEBUTTONDOWN:
		{
			_gui.UpdateMouse(p_event.motion.x,p_event.motion.y,ms);
			break;
		}
		case SDL_MOUSEMOTION:
		{
			_gui.UpdateMouse(p_event.motion.x,p_event.motion.y,0);
			if (ms&SDL_BUTTON(1)) // Left - rotation.
			{
				const float sens=100.0f;
				float dx=rmx/sens;
				float dy=rmy/sens;
				_ribbon.Dampen(0.25f);
				_ribbon.Rotate(dy,dx);
			}
			if (ms&SDL_BUTTON(3)) // Right - panning.
			{
				const float sens=10.0f;
				_ribbon.Pan(-rmx/sens,rmy/sens);
			}
			break;
		}

		case SDL_MOUSEWHEEL:
			_ribbon.Zoom(p_event.wheel.y/4.0f);
			break;
		case SDL_WINDOWEVENT:
			switch (p_event.window.event)
			{
				default:
					break;
				case SDL_WINDOWEVENT_SIZE_CHANGED:
					_gui.SetText(ControlName::STATUS,"Resized");
					break;
			}
			break;
		case SDL_KEYDOWN:
			if (p_event.key.keysym.sym==SDLK_UP)
			{
				_ribbon.Zoom(1.0f);
			}
			if (p_event.key.keysym.sym==SDLK_DOWN)
			{
				_ribbon.Zoom(-1.0f);
			}
			if (p_event.key.keysym.sym==SDLK_RIGHT)
			{
				ButtonNewCurve();
			}
			break;
	}
}

void App::Render()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	_ribbon.Render();
	_gui.Render();
}

bool App::Update()
{
	_timer+=0.016f;
	_ribbon.Update();
	return true;
}

//==============================================================================

void App::Init()
{
	glEnable(GL_CULL_FACE);
	// Blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	//glDepthRangef(0.f,1.f);
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	CheckGLErrors("App");
	// Interface
	_gui.Init();
	int menux=_displaySettings.width-96,
		menuy=_displaySettings.height/4,spacing=16;
	// Status bar - keep last
	_gui.SetPos(ControlName::STATUS,0,_displaySettings.height/2-16);
	_gui.SetSize(ControlName::STATUS,800,16);
	_gui.SetText(ControlName::STATUS,"RibbonGen");
	// Right-side buttons
	_gui.SetText(ControlName::NEWCURVE,"New curve");
	_gui.SetText(ControlName::RESETVIEW,"Reset view");
	_gui.SetText(ControlName::SAVE,"Save");
	for (int i=ControlName::NEWCURVE; i<=ControlName::SAVE; ++i)
	{
		_gui.SetPos(i,menux,menuy+i*spacing);
	}
	_gui.SetText(ControlName::HELP,"Help");
	_gui.SetPos(ControlName::HELP,menux,16);
	// Messagebox
	_gui.SetPos(ControlName::MESSAGE,32,menuy);
	_gui.SetSize(ControlName::MESSAGE,512,64);
	_gui.ToggleVisibility(MESSAGE);// hide it
	// Yes No
	_gui.SetPos(ControlName::YES,32,menuy+68);
	_gui.SetPos(ControlName::NO,456,menuy+68);
	_gui.SetText(ControlName::YES,"OK");
	_gui.SetText(ControlName::NO,"Cancel");
	_gui.ToggleVisibility(ControlName::YES);
	_gui.ToggleVisibility(ControlName::NO);
	// Binds
	_gui.Bind(ControlName::HELP,		std::bind(&App::ButtonHelp,this));
	_gui.Bind(ControlName::NEWCURVE,	std::bind(&App::ButtonNewCurve,this));
	_gui.Bind(ControlName::RESETVIEW,	std::bind(&App::ButtonReset,this));
	_gui.Bind(ControlName::SAVE,		std::bind(&App::ButtonSave,this));
	_gui.Bind(ControlName::YES,			std::bind(&App::ButtonYes,this));
	_gui.Bind(ControlName::NO,			std::bind(&App::ButtonNo,this));
	_gui.Bind(ControlName::MESSAGE,		std::bind(&App::ButtonMessage,this));
}


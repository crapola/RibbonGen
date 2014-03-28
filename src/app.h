#ifndef APP_H
#define APP_H

#include "displaysettings.hpp"
#include "interface.h"
#include "ribbon.h"

class SDL_Window;
union SDL_Event;

class App
{
public:
	App(SDL_Window*,DisplaySettings);
	~App();
	App(const App&)=delete;
	App& operator=(const App&)=delete;
	//--------------------------------------------------------------------------
	void ButtonHelp(),ButtonNewCurve(),ButtonReset(),ButtonSave(),ButtonYes(),
		 ButtonNo(),ButtonMessage();
	void ProcessEvent(const SDL_Event&);
	void Render();
	bool Update();
	//==========================================================================
private:
	void Init();

	SDL_Window* _screen;
	DisplaySettings _displaySettings;
	float _timer;
	Interface _gui;
	Ribbon _ribbon;
	enum UIState:int
	{
		S_BASE=0,
		SAVE=1,
		S_HELP
	} _uistate;
};

enum ControlName:int
{
	HELP=0,
	NEWCURVE=1,
	RESETVIEW,
	SAVE,
	UNUSED4,
	UNUSED5,
	YES,
	NO,
	MESSAGE,
	STATUS=9
};

#endif // APP_H

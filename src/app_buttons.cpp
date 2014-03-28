#include "app.h"

void App::ButtonHelp()
{
	if (_uistate==SAVE)
	{
		ButtonNo();
	}
	_uistate=S_HELP;
	_gui.ToggleVisibility(ControlName::MESSAGE);
	_gui.SetSize(ControlName::MESSAGE,512,8*12+4);
	_gui.SetText(ControlName::MESSAGE,
				 "Help\n\n"
				 "Manipulate the ribbon with your mouse.\n\n"
				 "[MOUSE+LEFT] Rotate\n"
				 "[MOUSE+RIGHT] Pan\n"
				 "[MOUSE WHEEL] Zoom\n\n"
				 "[ESC] or close the window to quit the application.\n"
				 "\n\nClick this box to close.");
}

void App::ButtonNewCurve()
{
	_ribbon.Build();
	_gui.SetText(ControlName::STATUS,"New curve.");
}

void App::ButtonReset()
{
	_ribbon.Reset();
	_gui.SetText(ControlName::STATUS,"View reset.");
}

void App::ButtonSave()
{
	if (_uistate==S_HELP)
	{
		_gui.ToggleVisibility(ControlName::MESSAGE);
	}
	_uistate=SAVE;
	_gui.ToggleVisibility(ControlName::MESSAGE);
	_gui.SetSize(ControlName::MESSAGE,512,8*7+4);
	_gui.SetText(ControlName::MESSAGE,
				 "Render to a 4096x4096 bitmap (64 MB).\n"
				 "Are you sure?");
	_gui.ToggleVisibility(ControlName::YES);
	_gui.ToggleVisibility(ControlName::NO);
}

void App::ButtonYes()
{
	const unsigned short RENDER_SIZE=512;
	std::string filename;
	filename=_ribbon.Save(RENDER_SIZE,RENDER_SIZE);
	_gui.ToggleVisibility(ControlName::MESSAGE);
	_gui.ToggleVisibility(ControlName::YES);
	_gui.ToggleVisibility(ControlName::NO);
	_uistate=S_BASE;
	filename.insert(0,"Saved to: ");
	_gui.SetText(ControlName::STATUS,filename);
}

void App::ButtonNo()
{
	_gui.ToggleVisibility(ControlName::YES);
	_gui.ToggleVisibility(ControlName::MESSAGE);
	_gui.ToggleVisibility(ControlName::NO);
	_uistate=S_BASE;
}

void App::ButtonMessage()
{
	if (_uistate==S_HELP)
	{
		_gui.ToggleVisibility(ControlName::MESSAGE);
		_uistate=S_BASE;
	}
}
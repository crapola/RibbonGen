#ifndef INTERFACE_H
#define INTERFACE_H

#include <functional>
#include <vector>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include "simpletext.h"

// draws bunch of text and flat recs.

class Interface
{
	typedef std::function<void()> callback_t;
public:
	Interface();
	~Interface();
	Interface(const Interface&)=delete;
	Interface& operator=(const Interface&)=delete;
	//--------------------------------------------------------------------------
	void Init();
	void Render();
	// controls
	void Bind(int control,const callback_t&);
	int HitTest(int x,int y);
	void SetPos(int control,int x,int y);
	void SetSize(int control,int w,int h);
	void SetText(int control,const std::string& text);
	void ToggleVisibility(int control);
	void UpdateMouse(Uint16,Uint16,Uint32);
private:
	void UpdateBuffers();
	//==========================================================================
	unsigned long _ticker;
	static const unsigned int MAX_CONTROLS=10,MAX_TXT_LEN=32;

	SimpleText _simpleText;
	std::vector<std::reference_wrapper<SimpleText::Text>> _texts;// TODO: ref the list?

	std::vector<callback_t> _callbacks;

	GLuint _vao;
	// recs
	GLuint _rectangleVBO,_rectangleShader;
	std::vector<GLfloat> _rectangle;		// vertex coords
	GLuint _rectangleColorsBO;
	std::vector<GLfloat> _rectangleColors;
	GLuint _rectangleDimensionsBO;
	std::vector<GLint> _rectangleDimensions;	// x,y,w,h * 10
	GLuint _rectangleSelectedBO;
	std::vector<GLbyte> _rectangleSelected;
};

#endif // INTERFACE_H

#include <stdexcept>
#include "interface.h"

#define CHECKB(x)\
if (__builtin_expect(static_cast<unsigned int>(x)>=MAX_CONTROLS,0))\
{\
std::cerr<<"Invalid control: "<<x<<std::endl;\
throw std::runtime_error("Invalid control");\
return;\
}

void Interface::Bind(int p_i,const callback_t& p_f)
{
	CHECKB(p_i);
	_callbacks[p_i]=p_f;
}

int Interface::HitTest(int p_x,int p_y)
{
	float mx=p_x/1.0f,my=p_y/2.0f; //TODO:disp settings
	for (size_t i=0; i<_rectangleDimensions.size(); i+=4)
	{
		if (mx>_rectangleDimensions[i]&&
				mx<(_rectangleDimensions[i]+_rectangleDimensions[i+2])&&
				my>_rectangleDimensions[i+1]&&
				my<(_rectangleDimensions[i+1]+_rectangleDimensions[i+3])
		   )
		{
			return i/4;
		}
	}
	return -1;
}

void Interface::SetPos(int p_i,int p_x,int p_y)
{
	CHECKB(p_i);
	_texts[p_i].get().SetPos(p_x+2,p_y+2);

	_rectangleDimensions[4*p_i]=p_x;
	_rectangleDimensions[4*p_i+1]=p_y;
	glBindBuffer(GL_ARRAY_BUFFER,_rectangleDimensionsBO);
	glBufferSubData(GL_ARRAY_BUFFER,
					p_i*4*sizeof(GLint),
					2*sizeof(GLint),
					&_rectangleDimensions[4*p_i]);
	glBindBuffer(GL_ARRAY_BUFFER,0);
}

void Interface::SetSize(int p_i,int p_w,int p_h)
{
	CHECKB(p_i);
	_rectangleDimensions[4*p_i+2]=p_w;
	_rectangleDimensions[4*p_i+3]=p_h;
	glBindBuffer(GL_ARRAY_BUFFER,_rectangleDimensionsBO);
	glBufferSubData(GL_ARRAY_BUFFER,
					2*sizeof(GLint)+p_i*4*sizeof(GLint),
					2*sizeof(GLint),
					&_rectangleDimensions[4*p_i+2]);
	glBindBuffer(GL_ARRAY_BUFFER,0);

	_texts[p_i].get().SetMargin(p_w/8-1);
}

void Interface::SetText(int p_line,const std::string& p_str)
{
	CHECKB(p_line);
	_texts[p_line].get().SetText(p_str);
}

void Interface::ToggleVisibility(int p_i)
{
	CHECKB(p_i);
	SetPos(p_i,_rectangleDimensions[4*p_i],-_rectangleDimensions[4*p_i+1]);
}

void Interface::UpdateMouse(Uint16 p_x,Uint16 p_y,Uint32 p_b)
{
	// Update selection effect
	//
	_rectangleSelected.assign(10,0);
	int c=HitTest(p_x,p_y);
	if (c!=-1)
	{
		_rectangleSelected[c]=1;

		if (p_b==1 && _callbacks[c]) _callbacks[c]();
	}
}

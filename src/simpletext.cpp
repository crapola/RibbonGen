#include "simpletext.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <SDL2/SDL.h>
#include "debuglog.hpp"
#include "quadstrip.hpp"
#include "checkglerrors.h"
#include "loadshaders.h"

#define CHAR_SCALE 8 // w & h
#define CHAR_RATIO (CHAR_SCALE/2048.0f) // % of texture
#define DATA_PER_CHAR 4

#define NULL_GLVOID (static_cast<GLvoid*>(0))

enum ObjectName:size_t
{
	VAO=0,
	VBO_DATA=1,
	IBO,
	SHADER,
	TEXTURE
};

SimpleText::SimpleText(size_t p_maxChars):
	_objects {0,0,0,0,0},
		 _coords(),
		 _const_vdata(_coords),
		 _MAX_CHARS(p_maxChars),
		 _MAX_TEXTS(),
		 _texts()
{
	CreateBuffers();
	CheckGLErrors("Text Buffers");
	PrepareTexture();
	CreateProgram();
	glBindVertexArray(0);
	CheckGLErrors("Text");
}

SimpleText::~SimpleText()
{
	if (glIsVertexArray(_objects[VAO]))
		glDeleteVertexArrays(1,&_objects[VAO]);

	if (glIsBuffer(_objects[VBO_DATA]))
		glDeleteBuffers(1,&_objects[VBO_DATA]);

	if (glIsBuffer(_objects[IBO]))
		glDeleteBuffers(1,&_objects[IBO]);

	if (glIsProgram(_objects[SHADER]))
		glDeleteProgram(_objects[SHADER]);

	if (glIsTexture(_objects[TEXTURE]))
		glDeleteTextures(1,&_objects[TEXTURE]);

	CheckGLErrors("~Text");
}

//------------------------------------------------------------------------------

SimpleText::TextPtr SimpleText::CreateText(const std::string& p_str,int p_x,
		int p_y,int p_w,int)
{
	std::string str(p_str);
	if (str.empty())
	{
		str=" ";
	}
	if (p_w<=0) p_w=1;
	// Create so offset at the end.
	size_t o=_const_vdata.size();
	// Special chars don't count toward data size.
	size_t l=str.size()-std::count_if(str.begin(),str.end(),[](unsigned char c)
	{
		return c<' ';
	});
	_texts.push_back(Text(this,o,l,p_w));
	_coords.resize(o+l*DATA_PER_CHAR);
	WriteParagraph(p_str,p_x,p_y,p_w,o);
	return _texts.back();
}

SimpleText::TextPtr SimpleText::CreateText(float p_str,int p_x,int p_y,
		int p_w,int p_h)
{
	std::ostringstream ss;
	ss<<p_str;
	return CreateText(ss.str(),p_x,p_y,p_w,p_h);
}

void SimpleText::Render()
{
	if (_const_vdata.Pending()) Refresh();
	if (_const_vdata.size()==0) return;
	glBindVertexArray(_objects[VAO]);
	glUseProgram(_objects[SHADER]);
	glDrawElements(GL_TRIANGLES,6*_const_vdata.size()/DATA_PER_CHAR,GL_UNSIGNED_SHORT,0);
	glUseProgram(0);
	glBindVertexArray(0);
}

//==============================================================================

inline float SimpleText::CharOffset(unsigned char p_c) const
{
	return (p_c-32)*CHAR_RATIO;
}

bool SimpleText::CreateBuffers()
{
	glGenVertexArrays(1,&_objects[VAO]);
	glBindVertexArray(_objects[VAO]);
	// later could replace 2 UVs by the one ASCII value, do on gpu
	// u=(c-32)*(8.0f/2048.0f) + ( (vertex_id%4)/2)*(8.0f/2048.0f)
	// v=(vertex_id%2)*(8.0f/2048.0f)
	glGenBuffers(1,&_objects[VBO_DATA]);
	glBindBuffer(GL_ARRAY_BUFFER,_objects[VBO_DATA]);
	glBufferData(GL_ARRAY_BUFFER,_MAX_CHARS*DATA_PER_CHAR*sizeof(VertexFormat),NULL,GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0,DATA_PER_CHAR,GL_FLOAT,GL_FALSE,0,NULL_GLVOID);
	glEnableVertexAttribArray(0);
	// 6 indices per char (0 1 2 1 3 0)
	std::vector<GLushort> indices;
	for (size_t i=0; i<_MAX_CHARS; ++i)
	{
		indices.push_back(i*4+0);
		indices.push_back(i*4+2);
		indices.push_back(i*4+1);
		indices.push_back(i*4+1);
		indices.push_back(i*4+2);
		indices.push_back(i*4+3);
	}
	glGenBuffers(1,&_objects[IBO]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,_objects[IBO]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size()*sizeof(GLushort),&indices[0],GL_STATIC_DRAW);
	return true;
}

bool SimpleText::CreateProgram()
{
	_objects[SHADER]=ProgramFromFiles("shaders/txt_V.glsl","shaders/txt_F.glsl");
	if (_objects[SHADER]==0)
	{
		return false;
	}
	glUseProgram(_objects[SHADER]);
	glUniform1i(0,1);
	return true;
}

bool SimpleText::PrepareTexture()
{
	const GLubyte* indata=static_cast<const GLubyte*>(font_data);
	GLubyte dest[2048*8]= {0};
	// Convert 1bpp to 1Bpp (GL_RED)
	for (int y=0; y<8; ++y)
		for (int x=0; x<224; ++x)
		{
			GLubyte c=indata[x+y*224];
			for (int b=0; b<8; ++b)
			{
				if (((c)&(1<<b))!=0)
				{
					dest[x*8+(7-y)*2048+7-b]=255;
				}

			}
		}
	glActiveTexture(GL_TEXTURE1);
	glGenTextures(1,&_objects[TEXTURE]);
	glBindTexture(GL_TEXTURE_2D,_objects[TEXTURE]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D,0,GL_COMPRESSED_RED,2048,8,0,GL_RED,GL_UNSIGNED_BYTE,dest);
	return true;
}

void SimpleText::Refresh()
{
	glBindBuffer(GL_ARRAY_BUFFER,_objects[VBO_DATA]);
	glBufferSubData(GL_ARRAY_BUFFER,_const_vdata.From()*sizeof(VertexFormat),
					_const_vdata.RangeSize()*sizeof(VertexFormat),
					&_const_vdata[0]+_const_vdata.From());
	glBindBuffer(GL_ARRAY_BUFFER,0);
	_coords.Reset();
}

void SimpleText::RemoveText(Text*) const
{
	// TODO: that
}

void SimpleText::SetMargin(Text& p_text,int p_m)
{
	size_t from=p_text._offset,
		   to=from+p_text._numChars*DATA_PER_CHAR;
	// Origin from first char.
	GLfloat xo=_const_vdata[from].x;
	GLfloat yo=_const_vdata[from].y;
	// Like writeparagraph but only positions..
	size_t destOffset=p_text._offset;
	int w=0,oldm=0;
	float x=xo,y=yo;
	for (size_t i=from; i<to; i+=DATA_PER_CHAR)
	{
		float cx=_const_vdata[destOffset].x;
		if (w==p_m)
		{
			w=0;
			y+=CHAR_SCALE;
			x=xo;
		}
		if (i>from && cx==xo) // line jump
		{
			if (oldm<p_text._margin) // actual newline
			{
				y+=CHAR_SCALE;
				x=xo;
				w=0;
			}
			else // nope
			{
				oldm=0;
			}
		}
		_coords[destOffset  ].x=x;
		_coords[destOffset  ].y=y;
		_coords[destOffset+1].x=x;
		_coords[destOffset+1].y=y;
		_coords[destOffset+2].x=x;
		_coords[destOffset+2].y=y;
		_coords[destOffset+3].x=x;
		_coords[destOffset+3].y=y;
		destOffset+=DATA_PER_CHAR;
		++w;
		x+=CHAR_SCALE;
		++oldm;
	}
	p_text._margin=p_m;
}

void SimpleText::SetPos(const Text& p_text,int p_x,int p_y)
{
	size_t from=p_text._offset, to=from+p_text._numChars*DATA_PER_CHAR;
	// Origin
	GLfloat x=_const_vdata[from].x;
	GLfloat y=_const_vdata[from].y;
	GLfloat dx=p_x-x,dy=p_y-y;
	for (size_t o=from; o<to; o+=DATA_PER_CHAR)
	{
		_coords[o  ].x+=dx;
		_coords[o  ].y+=dy;
		_coords[o+1].x+=dx;
		_coords[o+1].y+=dy;
		_coords[o+2].x+=dx;
		_coords[o+2].y+=dy;
		_coords[o+3].x+=dx;
		_coords[o+3].y+=dy;
	}
}

void SimpleText::SetText(Text& p_text,const std::string& p_str)
{
	int x=_const_vdata[p_text._offset].x,y=_const_vdata[p_text._offset].y;
	int m=p_text._margin;
	size_t newsize=p_str.size()-std::count_if(p_str.begin(),p_str.end(),[](unsigned char c)
	{
		return c<' ';
	});
	// Shrink
	if (newsize<p_text._numChars)
	{
		size_t diff=int(p_text._numChars)-newsize;
		_coords.erase(_coords.begin()+p_text._offset,
					  _coords.begin()+p_text._offset+diff*DATA_PER_CHAR);
		ShiftTexts(&p_text,-diff*DATA_PER_CHAR);
	}
	// Make room
	if (newsize>p_text._numChars)
	{
		size_t diff=newsize-p_text._numChars;
		_coords.insert(_coords.begin()+p_text._offset,diff*DATA_PER_CHAR,VertexFormat());
		ShiftTexts(&p_text,diff*DATA_PER_CHAR);
	}
	p_text._numChars=newsize;
	WriteParagraph(p_str,x,y,m,p_text._offset);
}

void SimpleText::ShiftTexts(const Text* p_istart,int p_diff)
{
	std::list<Text>::iterator it;
	it=std::find_if(_texts.begin(),_texts.end(), [=](const Text& t)
	{
		return &t==p_istart;
	});
	for (++it; it!=_texts.end(); ++it)
	{
		(*it)._offset+=p_diff;
	}
}

void SimpleText::WriteParagraph(const std::string& p_s,int p_x,int p_y,int p_m,
								size_t p_from)
{
	// At this point _coords is ready for writing.
	size_t destOffset=p_from;
	GLfloat x=p_x;
	GLfloat y=p_y;
	int w=0;
	for (size_t i=0; i<p_s.size(); ++i)
	{
		unsigned char c=p_s[i];
		if (c=='\n' || w==p_m)
		{
			w=0;
			y+=CHAR_SCALE;
			x=p_x;
		}
		if (c<' ')
		{
			continue;
		}
		GLfloat u=CharOffset(p_s[i]);
		GLfloat u2=u+CHAR_RATIO;
		_coords[destOffset  ].u=(u);//A
		_coords[destOffset  ].v=(1);
		_coords[destOffset  ].x=(x);
		_coords[destOffset  ].y=(y);
		_coords[destOffset+1].u=(u);//B
		_coords[destOffset+1].v=(0);
		_coords[destOffset+1].x=(x);
		_coords[destOffset+1].y=(y);
		_coords[destOffset+2].u=(u2);//C
		_coords[destOffset+2].v=(1);
		_coords[destOffset+2].x=(x);
		_coords[destOffset+2].y=(y);
		_coords[destOffset+3].u=(u2);//D
		_coords[destOffset+3].v=(0);
		_coords[destOffset+3].x=(x);
		_coords[destOffset+3].y=(y);
		destOffset+=DATA_PER_CHAR;
		x+=CHAR_SCALE;
		++w;
	}
	if (_const_vdata.size()>_MAX_CHARS)
	{
		std::cerr<<"SimpleText: Character budget overrun ("<<_coords.size()<<">"<<_MAX_CHARS<<").\n";
		_coords.erase(_coords.begin()+_MAX_CHARS,_coords.end());
	}
}

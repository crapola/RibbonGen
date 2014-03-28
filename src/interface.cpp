#include <cmath>
#include <iostream>

#include "interface.h"
#include "loadshaders.h"
#include "quadstrip.hpp"
#include "loadtexture.h"
#include "checkglerrors.h"

#define NULL_GLVOID (static_cast<GLvoid*>(0))

Interface::Interface():_ticker(),
	_simpleText(1536),
	_texts(),_callbacks(MAX_CONTROLS),
	_vao(),
	_rectangleVBO(0),_rectangleShader(0),_rectangle(),
	_rectangleColorsBO(0),_rectangleColors(MAX_CONTROLS*4),
	_rectangleDimensionsBO(0),_rectangleDimensions(),
	_rectangleSelectedBO(0),_rectangleSelected(MAX_CONTROLS,0)

{
	for (unsigned int i=0; i<MAX_CONTROLS*4-8; i+=8)
	{
		_rectangleColors[i]=0.05f;
		_rectangleColors[i+1]=0.2f;
		_rectangleColors[i+2]=0.5f;
		_rectangleColors[i+3]=0.25f;

		_rectangleColors[i+4]=0.4f;
		_rectangleColors[i+5]=0.8f;
		_rectangleColors[i+6]=1.0f;
		_rectangleColors[i+7]=1.0f;
	}
	QuadStrip2D<GLfloat>(_rectangle,1,1,1);
	for (int i=0; i<10; ++i)
	{
		_texts.push_back(_simpleText.CreateText("null",0,-10,20,0));
	}
}

Interface::~Interface()
{
	glDeleteVertexArrays(1,&_vao);

	glDeleteBuffers(1,&_rectangleVBO);
	glDeleteBuffers(1,&_rectangleColorsBO);
	glDeleteBuffers(1,&_rectangleDimensionsBO);
	glDeleteBuffers(1,&_rectangleSelectedBO);
	glDeleteProgram(_rectangleShader);

	CheckGLErrors("~Interface");
}

//==============================================================================

void Interface::Init()
{
	// vao
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	//--------------------------------------------------------------------------
	// Rectangle
	//--------------------------------------------------------------------------
	// shader
	_rectangleShader=ProgramFromFiles("shaders/rec_V.glsl","shaders/rec_F.glsl");

	// verts
	glGenBuffers(1,&_rectangleVBO);
	glBindBuffer(GL_ARRAY_BUFFER,_rectangleVBO);
	glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,0,NULL_GLVOID);
	glBufferData(GL_ARRAY_BUFFER,_rectangle.size()*sizeof(GLfloat),&_rectangle[0],GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);

	// colors
	glGenBuffers(1,&_rectangleColorsBO);
	glBindBuffer(GL_ARRAY_BUFFER,_rectangleColorsBO);
	glBufferData(GL_ARRAY_BUFFER,_rectangleColors.size()*sizeof(GLfloat),&_rectangleColors[0],GL_DYNAMIC_DRAW);
	glVertexAttribPointer(1,4,GL_FLOAT,GL_FALSE,0,NULL_GLVOID);
	glEnableVertexAttribArray(1);//color

	_rectangleDimensions.resize(MAX_CONTROLS*4);
	glGenBuffers(1,&_rectangleDimensionsBO);
	glBindBuffer(GL_ARRAY_BUFFER,_rectangleDimensionsBO);
	glVertexAttribIPointer(2,4,GL_INT,0,NULL_GLVOID);
	glEnableVertexAttribArray(2);
	glVertexAttribDivisor(2,1);//one per instance
	for (unsigned int i=0; i<MAX_CONTROLS; i++)
	{
		_rectangleDimensions[4*i]=0;
		_rectangleDimensions[4*i+1]=-20;
		_rectangleDimensions[4*i+2]=88;
		_rectangleDimensions[4*i+3]=12;
	}
	glBufferData(GL_ARRAY_BUFFER,_rectangleDimensions.size()*sizeof(GLint),&_rectangleDimensions[0],GL_DYNAMIC_DRAW);

	glGenBuffers(1,&_rectangleSelectedBO);
	glBindBuffer(GL_ARRAY_BUFFER,_rectangleSelectedBO);
	glVertexAttribIPointer(3,1,GL_BYTE,0,NULL_GLVOID);
	glEnableVertexAttribArray(3);
	glVertexAttribDivisor(3,1);
	glBufferData(GL_ARRAY_BUFFER,_rectangleSelected.size()*sizeof(GLbyte),&_rectangleSelected[0],GL_STATIC_DRAW);

	//--------------------------------------------------------------------------
	//done vao
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindVertexArray(0);

	CheckGLErrors("Interface");
}

void Interface::Render()
{
	UpdateBuffers();
	glBindVertexArray(_vao);
	// Render rects

	glUseProgram(_rectangleShader);
	// ticker uniform
	GLint u=glGetUniformLocation(_rectangleShader,"timer");
	glUniform1f(u,fabs(sin(_ticker/ 3.1415927f/8.0f)));

	glDrawArraysInstanced(GL_TRIANGLE_STRIP,0,4,10);


	glBindVertexArray(0);

	_simpleText.Render();
	_ticker++;
}

void Interface::UpdateBuffers()
{
	// Recs
	glBindBuffer(GL_ARRAY_BUFFER,_rectangleSelectedBO);
	glBufferSubData(GL_ARRAY_BUFFER,
					0,
					sizeof(GLbyte)*MAX_CONTROLS,
					&_rectangleSelected[0]);

}

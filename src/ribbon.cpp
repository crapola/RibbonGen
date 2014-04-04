#include <array>
#include <chrono>
#include <ctime>
#include <cstdint>
#include <iostream>
#include <random>
#include <stdexcept>
#include <tuple>

#include <SDL2/SDL.h>

#include "debuglog.hpp"
#include "checkglerrors.h"
#include "loadshaders.h"
#include "ribbon.h"
#include "serie.hpp"

#define NULL_GLVOID (static_cast<GLvoid*>(0))

// Dimensions of ribbon grid
#define RIBW 16  	// Span
#define RIBH 512	// Edge

#define RATIO 800.0f/600.0f

#define F_PI 3.14159265358979f

Ribbon::Ribbon():_vao(),_vbo(),_ibo(),_program(),_verts(),_indices(),
	_accx(),_accy(),_rotx(1.0f),_roty(),_damp(0.75f),_panx(),_pany(),
	_zoomLevel(2.0f),_generator(std::chrono::system_clock::now().time_since_epoch().count())
{
	const GLint VERT_COMPONENT_COUNT=sizeof(Vertex)/sizeof(GLfloat);
	static_assert(VERT_COMPONENT_COUNT==7,"Bad vertex format");
	// VAO
	glGenVertexArrays(1,&_vao);
	glBindVertexArray(_vao);
	// Program
	_program=ProgramFromFiles("shaders/ribbon_V.glsl","shaders/ribbon_F.glsl");
	// Buffer
	glGenBuffers(1,&_vbo);
	glBindBuffer(GL_ARRAY_BUFFER,_vbo);
	// Verts
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,
						  sizeof(GLfloat)*VERT_COMPONENT_COUNT,NULL_GLVOID);
	// Color
	glVertexAttribPointer(1,4,GL_FLOAT,GL_FALSE,
						  sizeof(GLfloat)*VERT_COMPONENT_COUNT,
						  reinterpret_cast<GLvoid*>(sizeof(GLfloat)*3));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	// Incides
	glGenBuffers(1,&_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,_ibo);
	for (int q=0; q<RIBW*RIBH; ++q) // for each quad
	{
		int a,b,c;
		int line=q/(RIBW);
		a=q+line;
		b=a+RIBW+1;
		c=a+1;
		_indices.push_back(a);
		_indices.push_back(b);
		_indices.push_back(c);
		a=b;
		b=b+1;
		//c is the same
		_indices.push_back(a);
		_indices.push_back(b);
		_indices.push_back(c);
	}
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,_indices.size()*sizeof(GLuint),
				 &_indices[0],
				 GL_STATIC_DRAW);
	Build();
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	CheckGLErrors("Ribbon");
}

Ribbon::~Ribbon()
{
	glDeleteVertexArrays(1,&_vao);
	glDeleteBuffers(1,&_vbo);
	glDeleteBuffers(1,&_ibo);
	glDeleteProgram(_program);
	CheckGLErrors("~Ribbon");
}

//------------------------------------------------------------------------------

void Ribbon::Build()
{
	std::uniform_real_distribution<float> dr;

	const int NC=16;
	GLfloat x=0,y=0,z=0,r,g,b,a,col;
	// Random parameters
	std::array<float,NC*3> coefs;

	//std::fill(coefs.begin(),coefs.end(),1);
	for (auto& e:coefs)
	{
		e=dr(_generator);
	}
	// Leading edge
	auto edgefun=[&coefs](float y) -> std::tuple<float,float,float>
	{
		return std::tuple<float,float,float>
		(
			SerieCos<NC>(y,coefs.data()),
			SerieCos<NC>(y+coefs[0],&coefs.data()[NC]),
			SerieCos<NC>(y+coefs[1],&coefs.data()[NC*2])
		);
	};

	// Span to color value x in [0,RIBW]
	auto colorfun=[&coefs](float x)
	{
		return 0.5+(1.0f-0.5)*sin((1.0f+4.0f*coefs[0])* (F_PI*x/RIBW));
	};

	// Build verts
	_verts.clear();
	for (int yy=0; yy<=RIBH; ++yy)
	{
		const float ANGLESTEP=2.0f*F_PI/RIBH;
		const float SCALE=1;
		std::tuple<float,float,float> plop=edgefun(yy*ANGLESTEP);
		x=std::get<0>(plop);
		y=std::get<1>(plop);
		z=std::get<2>(plop);
		// Extrude

		float len=sqrt(x*x+y*y+z*z)*25.f;
		float directx=-x/len;
		float directy=-y/len;
		float directz=-z/len;
		for (int xx=0; xx<=RIBW; ++xx)
		{
			col=colorfun(xx);
			r=1.0f-float(RIBW-xx)/float(RIBW);
			g=r;//col*0.8;
			b=1;//col*0.0125;
			a=col;
			_verts.push_back(Vertex {SCALE*(directx*xx+x),
									 SCALE*(directy*xx+y),
									 SCALE*(directz*xx+z),
			{r,g,b,a}
									});
		}

	}
	// Update verts
	glBindBuffer(GL_ARRAY_BUFFER,_vbo);
	glBufferData(GL_ARRAY_BUFFER,
				 _verts.size()*sizeof(Vertex),
				 &_verts[0],
				 GL_STATIC_DRAW);
}

void Ribbon::Dampen(float p_d)
{
	_damp=p_d;
}

void Ribbon::Pan(float p_dx,float p_dy)
{
	_panx+=p_dx;
	_pany+=p_dy;
}

void Ribbon::Render() const
{
	glBindVertexArray(_vao);
	glUseProgram(_program);
	GLint v=glGetUniformLocation(_program,"angles");
	GLint pu=glGetUniformLocation(_program,"pan");
	GLint zob=glGetUniformLocation(_program,"zoom");
	glUniform2f(v,_rotx,_roty);
	glUniform2f(pu,_panx,_pany);
	glUniform1f(zob, 1.f/powf(2,_zoomLevel));
	glDrawElements(GL_TRIANGLES,_indices.size(),GL_UNSIGNED_INT,0);
	glBindVertexArray(0);
}

void Ribbon::Reset()
{
	_accx=_accy=_roty=_panx=_pany=0.0f;
	_zoomLevel=3.0f;
	_rotx=1.0f;
}

void Ribbon::Rotate(float p_ax,float p_ay)
{
	_accx=p_ax;
	_accy=p_ay;
}

std::string Ribbon::Save(unsigned short p_render_w,
						 unsigned short p_render_h) const
{
	if (__builtin_expect(p_render_w>8192||p_render_h>8192,0))
	{
		throw std::runtime_error("Excessive render size.");
		return 0;
	}
	DOUT("Saving.");
	// Create buffers
	GLuint fbo,rbo;
	glGenFramebuffers(1,&fbo);
	glGenRenderbuffers(1,&rbo);
	glBindRenderbuffer(GL_RENDERBUFFER,rbo);
	glRenderbufferStorage(GL_RENDERBUFFER,
						  GL_RGBA,
						  p_render_w,
						  p_render_h);
	// Attach
	glBindFramebuffer(GL_FRAMEBUFFER,fbo);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER,
							  GL_COLOR_ATTACHMENT0,
							  GL_RENDERBUFFER,
							  rbo);
	// Render
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0,0,p_render_w,p_render_h);
	Render();
	glViewport(0,0,800,600);
	// Save to file
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	std::vector<uint_fast8_t> pixels;
	pixels.resize(p_render_w*p_render_h*4);
	glReadPixels(0,0,p_render_w,p_render_h,GL_RGBA,GL_UNSIGNED_BYTE,&pixels[0]);
	CheckGLErrors("read");
	SDL_Surface* image=SDL_CreateRGBSurfaceFrom(&pixels[0],
					   p_render_w,
					   p_render_h,
					   32,
					   p_render_w*4,
					   0x000000FF,
					   0x0000FF00,
					   0x00FF0000,
					   0xFF000000);
	std::string filename=CreateFileName();
	SDL_SaveBMP(image,filename.c_str());
	SDL_FreeSurface(image);
	// Detach and cleanup
	glBindRenderbuffer(GL_RENDERBUFFER,0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);
	glDeleteFramebuffers(1,&fbo);
	glDeleteRenderbuffers(1,&rbo);
	CheckGLErrors("Ribbon saving");
	return filename;
}

void Ribbon::Update()
{
	_rotx+=_accx;
	_roty+=_accy;
	_accx*=_damp;
	_accy*=_damp;
	const float MINAC=0.00390625f;
	_accx=fabs(_accx)<MINAC?0:_accx;
	_accy=fabs(_accy)<MINAC?0:_accy;
}

void Ribbon::Zoom(float p_z)
{
	_zoomLevel+=p_z;
}
//------------------------------------------------------------------------------

std::string Ribbon::CreateFileName() const
{
	std::time_t t=std::time(0);
	std::tm tm=*std::localtime(&t);
	char buf[11]= {};
	if (std::strftime(buf,11,"%m%d%H%M%S",&tm)==0)
	{
		std::cerr<<"Error with strftime!\n";
		return "lastrender.bmp";
	}
	std::string f="render_"+std::string(buf)+".bmp";
	return f;
}

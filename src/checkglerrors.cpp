#include <iostream>
#include <GL/glew.h>


bool CheckGLErrors(const char* p_message)
{
	GLenum e=glGetError();

	std::cerr<<p_message<<": ";
	if (e==GL_NO_ERROR)
	{
		std::cerr<<"OK\n";
		return false;
	}

	while (e!=GL_NO_ERROR)
	{
		switch (e)
		{
			case GL_INVALID_ENUM:
				std::cerr<<"GL_INVALID_ENUM";
				break;
			case GL_INVALID_VALUE:
				std::cerr<<"GL_INVALID_VALUE";
				break;
			case GL_INVALID_OPERATION:
				std::cerr<<"GL_INVALID_OPERATION";
				break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				std::cerr<<"GL_INVALID_FRAMEBUFFER_OPERATION";
				break;
			case GL_OUT_OF_MEMORY:
				std::cerr<<"GL_OUT_OF_MEMORY";
				break;
			case GL_STACK_UNDERFLOW:
				std::cerr<<"GL_STACK_UNDERFLOW";
				break;
			case GL_STACK_OVERFLOW:
				std::cerr<<"GL_STACK_OVERFLOW";
				break;
			default:
				std::cerr<<"?Unknown enum";
				break;
		}
		e=glGetError();
	}
	std::cerr<<std::endl;
	return true;
}

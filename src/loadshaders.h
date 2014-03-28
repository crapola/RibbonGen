#ifndef LOADSHADERS_H_INCLUDED
#define LOADSHADERS_H_INCLUDED

#include <GL/glew.h>

// Create a shader program from shader text files
GLuint ProgramFromFiles(const char* vertFile,const char* fragFile);

// Same with strings
GLuint ProgramFromMemory(const char* vertString,const char* fragString);

#endif // LOADSHADERS_H_INCLUDED

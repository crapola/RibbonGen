#include <fstream>
#include <iostream>
#include <vector>
#include <GL/glew.h>

// Compile shader from string p_code. Result in p_out_shaderID. True on sucess.
bool CompileShader(const char* p_code,GLuint& p_out_shaderID)
{
	glShaderSource(p_out_shaderID,1,&p_code,NULL);
	glCompileShader(p_out_shaderID);
	// Check
	GLint result=GL_FALSE;
	glGetShaderiv(p_out_shaderID,GL_COMPILE_STATUS,&result);
	if (result==GL_TRUE)
		return true;

	std::cerr<<"Shader compilation failed.\n"<<p_code;
	int logLength;
	glGetShaderiv(p_out_shaderID,GL_INFO_LOG_LENGTH,&logLength);
	std::vector<char> VertexShaderErrorMessage(logLength);
	glGetShaderInfoLog(p_out_shaderID,logLength,NULL,
					   &VertexShaderErrorMessage[0]);
	std::cerr<<&VertexShaderErrorMessage[0]<<std::endl;
	return false;
}

bool LoadCompileShader(const char* p_f,GLuint& p_sid)
{
	std::string shaderCode;
	std::ifstream file(p_f,std::ios::in);
	if (file.is_open())
	{
		std::string line="";
		while (getline(file,line))
			shaderCode+="\n"+line;
		file.close();
	}
	else
	{
		std::cerr<<p_f<<" not found.\n";
		return false;
	}
	// Compile Shader
	std::cout<<"Compiling shader: "<<p_f<<".\n";
	return CompileShader(shaderCode.c_str(),p_sid);
}

GLuint ProgramFromFiles(const char* p_vertFile,const char* p_fragFile)
{
	// Create the shaders
	GLuint vertexShaderID=glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID=glCreateShader(GL_FRAGMENT_SHADER);

	if (LoadCompileShader(p_vertFile,vertexShaderID)&&
			LoadCompileShader(p_fragFile,fragmentShaderID))
	{
		// Link the program
		std::cout<<"Linking program.\n";
		GLuint ProgramID = glCreateProgram();
		glAttachShader(ProgramID, vertexShaderID);
		glAttachShader(ProgramID, fragmentShaderID);
		glLinkProgram(ProgramID);
		// Check the program
		GLint result=GL_FALSE;
		int logLength;
		glGetProgramiv(ProgramID,GL_LINK_STATUS,&result);
		glGetProgramiv(ProgramID,GL_INFO_LOG_LENGTH,&logLength);
		std::vector<char> ProgramErrorMessage(std::max(logLength,int(1)));
		glGetProgramInfoLog(ProgramID,logLength,NULL,&ProgramErrorMessage[0]);
		std::cerr<<&ProgramErrorMessage[0]<<std::endl;
		// Quit
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
		return ProgramID;
	}
	else
	{
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
		return 0;
	}
}

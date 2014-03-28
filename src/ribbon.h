#ifndef RIBBON_H
#define RIBBON_H

#include <random>
#include <string>
#include <vector>
#include <GL/glew.h>

class Ribbon
{
public:
	Ribbon();
	~Ribbon();
	Ribbon(const Ribbon&)=delete;
	Ribbon& operator=(const Ribbon&)=delete;
	void Build();
	void Dampen(float rotation_dampening);
	void Pan(float dx,float dy);
	void Render() const;
	void Reset();
	void Rotate(float dx,float dy);
	std::string Save(unsigned short render_width,
					 unsigned short render_height) const;
	void Update();
	void Zoom(float zoomLevel);
private:
	std::string CreateFileName() const;
	// GL
	struct Vertex
	{
		GLfloat x,y,z;
		struct Color
		{
			GLfloat r,g,b,a;
		} color;
	};
	GLuint _vao,_vbo,_ibo,_program;
	std::vector<Vertex> _verts;
	std::vector<GLuint> _indices;
	// Rotation
	float _accx,_accy,_rotx,_roty;
	float _damp;
	// Panning
	float _panx,_pany;
	// Zoom
	float _zoomLevel;

	std::default_random_engine _generator;
};

#endif // RIBBON_H

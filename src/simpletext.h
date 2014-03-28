#ifndef SIMPLETEXT_H
#define SIMPLETEXT_H

#include <list>
#include <string>
#include <GL/glew.h>
#include "svector.hpp"

class SimpleText
{
public:
	class Text
	{
		friend class SimpleText;
	public:
		Text(Text&&)=default;

		~Text();
		void Remove();
		void SetMargin(int w);
		void SetPos(int x,int y);
		void SetText(const std::string&);
	private:
		Text(const Text&)=default;
		Text& operator=(const Text&)=default;
		Text& operator=(Text&&)=default;

		Text(SimpleText*,size_t,size_t,int);

		SimpleText* _simpleText;
		size_t _offset,_numChars;
		int _margin;
	};
	typedef Text& TextPtr;

	SimpleText(size_t character_limit);
	~SimpleText();
	SimpleText(const SimpleText&)=delete;
	SimpleText& operator=(const SimpleText&)=delete;

	// h is unused
	TextPtr CreateText(const std::string& text,int x,int y,int w,int h);
	TextPtr CreateText(float text,int x,int y,int w,int h);
	void Render();
private:
	float CharOffset(unsigned char) const;
	bool CreateBuffers();
	bool CreateProgram();
	bool PrepareTexture();
	void Refresh();
	void RemoveText(Text*) const;
	void SetMargin(Text&,int);
	void SetPos(const Text&,int,int);
	void SetText(Text&,const std::string&);
	void ShiftTexts(const Text*,int);
	void WriteParagraph(const std::string&,int,int,int,size_t);

	GLuint _objects[5];

	struct VertexFormat
	{
		GLfloat u,v,x,y;
	};

	Svector<VertexFormat> _coords;
	const Svector<VertexFormat>& _const_vdata;

	const size_t _MAX_CHARS,_MAX_TEXTS;
	std::list<Text> _texts;

	static const unsigned char font_data[];
	static const std::string _SHADERS[];
};

#endif // SIMPLETEXT_H

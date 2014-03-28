#include "simpletext.h"

#include <iostream>

SimpleText::Text::Text(SimpleText* p_a,size_t p_o,size_t p_s,int p_m):
	_simpleText(p_a),
	_offset(p_o),
	_numChars(p_s),
	_margin(p_m)
{

}
SimpleText::Text::~Text()
{
	_simpleText->RemoveText(this);
}

//------------------------------------------------------------------------------

void SimpleText::Text::Remove()
{
	_simpleText->RemoveText(this);
}

void SimpleText::Text::SetMargin(int p_m)
{
	if (p_m<1) return;
	_simpleText->SetMargin(*this,p_m);
}

void SimpleText::Text::SetPos(int p_x,int p_y)
{
	_simpleText->SetPos(*this,p_x,p_y);
}

void SimpleText::Text::SetText(const std::string& p_str)
{
	_simpleText->SetText(*this,p_str);
}

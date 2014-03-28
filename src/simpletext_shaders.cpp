#include "simpletext.h"

const std::string SimpleText::_SHADERS[]=
{
"#version 330 core\n"
"in vec2 uv_frag;\n"
"out vec4 color;\n"
"layout(location=0) uniform sampler2D tex;\n"
"void main()\n"
"{\n"
"	//color=texture(tex,uv_frag);\n"
"	//color=vec4(1,1,1,0.5+texture(tex,uv_frag).r/2.f);\n"
"	color=vec4(1,1,1,0.1+texture(tex,uv_frag).r);\n"
"}"

,

"#version 330 core\n"
"#define WD (800/2)\n"
"#define HD (600/4)\n"
"layout(location=0) in vec4 data_in;\n"
"layout(location=1) in ivec2 pos_in;\n"
"out vec2 uv_frag;\n"
"const mat4x2 m=mat4x2(0,1, 0,0, 1,1, 1,0);\n"
"const float scale=8;\n"
"const vec2 vs[6]=vec2[6](\n"
"vec2(0,1),\n"
"vec2(0,0),\n"
"vec2(1,1),\n"
"vec2(0,0),vec2(1,0),vec2(1,1));\n"
"void main()\n"
"{\n"
"	vec2 vert=scale*m[gl_VertexID%4];\n"
"	//vec2 vert=scale*vs[gl_VertexID%6];\n"
"	vec2 pos=vert+data_in.zw;//pos_in;\n"
"	gl_Position=vec4((pos.x)/WD-1,1-(pos.y)/HD,0,1);\n"
"	uv_frag=data_in.xy;\n"
"}"
};
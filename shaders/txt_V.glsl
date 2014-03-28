#version 330 core

#define WD (800/2)
#define HD (600/4)

layout(location=0) in vec4 data_in;
layout(location=1) in ivec2 pos_in;

out vec2 uv_frag;

const mat4x2 m=mat4x2(0,1, 0,0, 1,1, 1,0);
const float scale=8;

const vec2 vs[6]=vec2[6](
vec2(0,1),
vec2(0,0),
vec2(1,1),

vec2(0,0),vec2(1,0),vec2(1,1));

void main()
{
	vec2 vert=scale*m[gl_VertexID%4];
	//vec2 vert=scale*vs[gl_VertexID%6];
	vec2 pos=vert+data_in.zw;//pos_in;
	gl_Position=vec4((pos.x)/WD-1,1-(pos.y)/HD,0,1);
	uv_frag=data_in.xy;
}
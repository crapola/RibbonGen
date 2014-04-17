#version 330 core

#define WD (800/2)
#define HD (600/4)

layout(location=0) in vec2 verts;
layout(location=1) in vec4 color;
layout(location=2) in ivec4 data;// xywh
layout(location=3) in int selectedin;

out vec4 color_frag;

layout(location=0) uniform float timer;

void main()
{
	// pos
	vec2 endpos=vec2(data.z*verts.x+data.x, data.w*verts.y+data.y);
	gl_Position= vec4( (endpos.x)/WD-1,1-(endpos.y)/HD,0,1);

	// color
	vec4 red=-color+vec4(1,color.g,color.b/2,0.5+color.a*timer/2.0f);
	color_frag=color+selectedin*red;
}
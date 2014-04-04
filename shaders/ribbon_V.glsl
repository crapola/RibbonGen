#version 330 core

layout(location=0) in vec3 pos;
layout(location=1) in vec4 color;

out vec4 color_frag;

const mat4 pers=mat4(1,0,0,0,
					 0,1.33,0,0,// 800/600 ratio
					 0,0,1,1,
					 0,0,0,0);

// Explicit locations only work on GL 4.3+
layout(location=0) uniform vec2 angles;
uniform vec2 pan;
uniform float zoom;

mat4 rotate(float x,float y)
{
	const mat4 rotx=mat4(1,0,0,0,
				   0,cos(x),-sin(x),0,
				   0,sin(x),cos(x),0,
				   0,0,0,1);
	const mat4 roty=mat4(cos(y),0,sin(y),0,
				   0,1,0,0,
				   -sin(y),0,cos(y),0,
				   0,0,0,1);
	return roty*rotx;
}

void main()
{
	vec4 offset=vec4(pan,15,10);
	gl_Position=pers*( offset + rotate(angles.x,angles.y)*zoom*vec4(pos+vec3(0,0,0),0) );
	color_frag=color;
}

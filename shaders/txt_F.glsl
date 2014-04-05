#version 330 core

in vec2 uv_frag;
out vec4 color;

layout(location=0) uniform sampler2D tex;

void main()
{
	//color=texture(tex,uv_frag);
	//color=vec4(1,1,1,0.5+texture(tex,uv_frag).r/2.f);
	color=vec4(1,1,1,0.1+texture(tex,uv_frag).r);
	gl_FragDepth=0;
}
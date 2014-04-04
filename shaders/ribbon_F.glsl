#version 330 core

in vec4 color_frag;
out vec4 color;

void main()
{
	color=color_frag;
	float z=gl_FragCoord.w;
	color=vec4(z,z,z,1);
}
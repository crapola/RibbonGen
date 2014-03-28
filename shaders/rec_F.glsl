#version 330 core

in vec4 color_frag;
out vec4 color;

//layout(origin_upper_left) in vec4 gl_FragCoord;

void main()
{
	//vec4 cy=color_frag-mod(color_frag,0.25*mod(gl_FragCoord.y,8) );
	//color = cy;
	color=color_frag;
}
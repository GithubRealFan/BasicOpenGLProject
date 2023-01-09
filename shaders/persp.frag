#version 400

in  vec4 vert_Color;
out vec4 frag_Color;

void main(void)
{
	frag_Color = vert_Color;
}

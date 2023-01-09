#version 400

layout(location=0) in vec4 in_Position;
layout(location=1) in vec4 in_Color;
layout(location=2) in vec4 in_Normal;

out vec4 vert_Pos;
out vec4 vert_Color;
out vec4 vert_Normal;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main(void)
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4( in_Position.xyz, 1.0 );
	
	vert_Pos    = in_Position;
	vert_Color  = in_Color;
	vert_Normal = in_Normal;
}

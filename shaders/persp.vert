#version 400

layout(location=0) in vec4 in_Position;
layout(location=1) in vec4 in_Color;
out vec4 vert_Color;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main(void)
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4( in_Position.xyz, 1.0 );
	vert_Color = in_Color;
}

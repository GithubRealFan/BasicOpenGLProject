#version 400

uniform mat4 u_projectionMatrix;
uniform mat4 u_modelViewMatrix;
uniform mat3 u_normalMatrix;

layout(location=0) in vec4 a_vertex;
layout(location=1) in vec4 a_color;
layout(location=2) in vec4 a_normal;

out vec3 v_normal;
out vec3 v_eye;
out vec4 v_color;

void main()
{
	vec4 vertex = u_modelViewMatrix * a_vertex;

	gl_Position = u_projectionMatrix * vertex;

	v_eye = -vec3(vertex);
	v_normal = mat3(u_modelViewMatrix) * vec3(a_normal);
	v_color = a_color;
}
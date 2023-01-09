#version 400

uniform mat4 u_projectionMatrix;
uniform mat4 u_modelViewMatrix;
uniform mat3 u_normalMatrix;

layout(location=0) in vec4 a_vertex;
layout(location=1) in vec4 a_color;
layout(location=2) in vec4 a_normal;

struct LightProperties
{
  vec3 direction;
  vec4 ambientColor;
  vec4 diffuseColor;
  vec4 specularColor;
};

struct MaterialProperties
{
  vec4 ambientColor;
  vec4 diffuseColor;
  vec4 specularColor;
  float specularExponent;
};

uniform LightProperties u_light;
uniform MaterialProperties u_material;

attribute vec3 position;
attribute vec3 normal;
uniform mat4 projection, modelview, normalMat;
varying vec3 normalInterp;
varying vec3 vertPos;
uniform int mode;   // Rendering mode
uniform float Ka;   // Ambient reflection coefficient
uniform float Kd;   // Diffuse reflection coefficient
uniform float Ks;   // Specular reflection coefficient
uniform float shininessVal; // Shininess
// Material color
uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform vec3 lightPos; // Light position
varying vec4 color; //color

void main()
{
  vec4 vertex = u_modelViewMatrix * a_vertex;
  gl_Position = u_projectionMatrix * vertex;

  v_normal = u_normalMatrix * vec3(a_normal);
  vertPos = vec3(vertex) / vertex.w;

  vec3 N = normalize(v_normal);
  vec3 L = normalize(lightPos - vertPos);
  // Lambert's cosine law
  float lambertian = max(dot(N, L), 0.0);
  float specular = 0.0;
  if(lambertian > 0.0) {
    vec3 R = reflect(-L, N);      // Reflected light vector
    vec3 V = normalize(-vertPos); // Vector to viewer
    // Compute the specular term
    float specAngle = max(dot(R, V), 0.0);
    specular = pow(specAngle, shininessVal);
  }
  color = vec4(Ka * ambientColor +
               Kd * lambertian * diffuseColor +
               Ks * specular * specularColor, 1.0);

  // only ambient
  if(mode == 2) color = vec4(Ka * u_material.ambientColor, 1.0);
  // only diffuse
  if(mode == 3) color = vec4(Kd * lambertian * u_material.diffuseColor, 1.0);
  // only specular
  if(mode == 4) color = vec4(Ks * specular * specularColor, 1.0);
}

#version 400

in vec4 vert_Pos;
in vec4 vert_Color;
in vec4 vert_Normal;

out vec4 frag_Color;

uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

vec4 shade( void )
{
	vec4 la = vec4( 0.5, 0.5, 0.5, 1.0 );
	vec4 ld = vec4( 0.7, 0.7, 0.7, 1.0 );
	vec4 ls = vec4( 1.0, 1.0, 1.0, 1.0 );

	vec4 ka = vert_Color;
	vec4 kd = vert_Color;
	vec4 ks = vec4( 1.0, 1.0, 1.0, 1.0 );

	float shininess = 32.0f;

	mat4 transf = viewMatrix * modelMatrix;

	vec3 FragPos  = vec3( transf * vert_Pos );
	vec3 FragNorm = mat3( transpose( inverse( transf ) ) ) * vert_Normal.xyz;
	vec3 LightPos = vec3( transf * vec4( 3.0, 0.0, 3.0, 1.0 ) );

	vec3 N = normalize( FragNorm ); // vertex normal
	vec3 L = normalize( LightPos - FragPos ); // light direction
	vec3 R = normalize( reflect( -L, N ) ); // reflected ray
	vec3 V = normalize( vec3( 0.0, 0.0, 1.0 ) ); // view direction

	float dotLN = dot( L, N );
	vec4 amb = ka * la;
	vec4 dif = kd * ld * max( dotLN, 0.0 );
	vec4 spe = ks * ls * pow( max( dot( V, R ), 0.0 ), shininess ) * max( dotLN, 0.0 );

	return amb + dif + spe;
}

void main( void )
{
	frag_Color = shade();
}

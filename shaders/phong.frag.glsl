#version 400

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

uniform	LightProperties u_light;
uniform	MaterialProperties u_material;

in vec3 v_normal;
in vec3 v_eye;
in vec4 v_color;

out vec4 fragColor;

void main()
{
	// Note: All calculations are in camera space.

	vec4 color = u_light.ambientColor * v_color;//u_material.ambientColor;

	vec3 normal = normalize(v_normal);

	vec3 light_direction = normalize(u_light.direction);

	float dotv = dot(light_direction, normal);

	vec3 sgn = cross(light_direction, normal);
	//if (sgn.z < 0)
	//	dotv = -abs(dotv);
	//else
	//	dotv = abs(dotv);

	float nDotL = max(0, 0.5+dotv/2);
		
	if (nDotL > 0.0)
	{
		vec3 eye = normalize(v_eye);
	
		// Incident vector is opposite light direction vector.
		vec3 reflection = reflect(-u_light.direction, normal);
		
		float eDotR = max(dot(eye, reflection), 0.0);
	
		color += u_light.diffuseColor * v_color * nDotL;//u_material.diffuseColor * nDotL;
		
		float specularIntensity = 0.0;
		
		if (eDotR > 0.0)
		{
			specularIntensity = pow(eDotR, u_material.specularExponent);
		}
		
		color += u_light.specularColor * u_material.specularColor * specularIntensity;
	}

	fragColor = color;
}   
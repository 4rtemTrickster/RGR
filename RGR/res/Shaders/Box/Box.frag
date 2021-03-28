#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec3 LightPos;

layout(location = 0) out vec4 FragColor;

struct Material
{
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
	float Shininess;
};

struct Light
{
	vec3 Possition;

	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
};

uniform Material u_Material;
uniform Light u_Light;


void main()
{
	// Ambient
	vec3 Ambient = u_Light.Ambient * u_Material.Ambient;

	// Diffuse
	vec3 Normalized = normalize(Normal);
	vec3 LightDir = normalize(LightPos - FragPos);
	float Dif = max(dot(Normalized, LightDir), 0.0);
	vec3 Diffuse = u_Light.Diffuse * (Dif * u_Material.Diffuse);

	//Specular
	vec3 ViewDir = normalize(-FragPos);
	vec3 ReflectDir = reflect(-LightDir, Normalized);
	float Spec = pow(max(dot(ViewDir, ReflectDir), 0.0), u_Material.Shininess);
	vec3 Specular = u_Light.Specular * (Spec * u_Material.Specular);

	FragColor = vec4(Ambient + Diffuse + Specular, 1.0f);
}
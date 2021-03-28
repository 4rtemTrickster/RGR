#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec3 LightPos;
in vec2 TexCoords;

layout(location = 0) out vec4 FragColor;

struct Material
{
	sampler2D Diffuse;
	sampler2D Specular;
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
	vec3 Ambient = u_Light.Ambient * vec3(texture(u_Material.Diffuse, TexCoords));

	// Diffuse
	vec3 Normalized = normalize(Normal);
	vec3 LightDir = normalize(LightPos - FragPos);
	float Dif = max(dot(Normalized, LightDir), 0.0);
	vec3 Diffuse = u_Light.Diffuse * Dif * vec3(texture(u_Material.Diffuse, TexCoords));

	//Specular
	vec3 ViewDir = normalize(-FragPos);
	vec3 ReflectDir = reflect(-LightDir, Normalized);
	float Spec = pow(max(dot(ViewDir, ReflectDir), 0.0), u_Material.Shininess);
	vec3 Specular = u_Light.Specular * Spec * vec3(texture(u_Material.Specular, TexCoords));

	FragColor = vec4(Ambient + Diffuse + Specular, 1.0f);
}
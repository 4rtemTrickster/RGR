#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec3 LightPos;

layout(location = 0) out vec4 color;

uniform vec3 u_ObjectColor;
uniform vec3 u_LightColor;

void main()
{
	// Ambient
	float AmbientStrength = 0.1f;
	vec3 Ambient = AmbientStrength * u_LightColor;

	// Diffuse
	vec3 Normalized = normalize(Normal);
	vec3 LightDir = normalize(LightPos - FragPos);
	float Dif = max(dot(Normalized, LightDir), 0.0);
	vec3 Diffuse = Dif * u_LightColor;

	//Specular
	float SpecularStrength = 0.5f;
	vec3 ViewDir = normalize(-FragPos);
	vec3 ReflectDir = reflect(-LightDir, Normalized);
	float Spec = pow(max(dot(ViewDir, ReflectDir), 0.0), 32);
	vec3 Specular = SpecularStrength * Spec * u_LightColor;

	color = vec4((Ambient + Diffuse + Specular) * u_ObjectColor, 1.0f);
}
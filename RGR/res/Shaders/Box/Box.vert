#version 330 core

layout(location = 0) in vec4 InPosition;
layout(location = 1) in vec3 InNormal;
layout(location = 2) in vec2 InTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec3 LightPos;
out vec2 TexCoords;

uniform vec3 u_LightPos;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
	gl_Position = u_Projection * u_View * u_Model * InPosition;

	TexCoords = InTexCoords;
	FragPos = vec3(u_View * u_Model * InPosition);
	Normal = mat3(transpose(inverse(u_View * u_Model))) * InNormal;
	LightPos = vec3(u_View * vec4(u_LightPos, 1.0));
}
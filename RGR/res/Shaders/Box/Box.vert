#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;

out vec3 FragPos;
out vec3 Normal;
out vec3 LightPos;

uniform vec3 u_LightPos;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
	gl_Position = u_Projection * u_View * u_Model * position;

	FragPos = vec3(u_View * u_Model * position);
	Normal = mat3(transpose(inverse(u_View * u_Model))) * normal;
	LightPos = vec3(u_View * vec4(u_LightPos, 1.0));
}
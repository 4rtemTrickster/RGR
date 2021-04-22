#pragma once

#include "Help.h"

#include "GL/VertexArray/VertexArray.h"
#include "GL/VertexArray/VertexBufferLayout.h"
#include "GL/IndexBuffer/IndexBuffer.h"


struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

//struct Texture
//{
//	GLuint id;
//	std::string type;
//};

class Mesh
{
public:
	Mesh(Mesh&& other) noexcept;

private:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	//std::vector<Texture> textures;

	GL::VertexBuffer vbo;
	GL::VertexBufferLayout layout;
	
public:
	GL::VertexArray vao;
	GL::IndexBuffer ibo;

	Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices/*, std::vector<Texture> textures*/);
};
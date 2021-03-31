#pragma once

#include "../../Help.h"

#include "../../GL/VertexArray/VertexArray.h"
#include "../../GL/VertexArray/VertexBufferLayout.h"
#include "../../GL/IndexBuffer/IndexBuffer.h"

#include "glm/glm.hpp"


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
private:
	GL::VertexArray vao;
	GL::VertexBuffer vbo;
	GL::VertexBufferLayout layout;
	GL::IndexBuffer ibo;
	
public:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	//std::vector<Texture> textures;

	Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices/*, std::vector<Texture> textures*/);
};
#include "Help.h"
#include "Mesh.h"

Mesh::Mesh(Mesh&& other) noexcept
: vertices(std::move(other.vertices)),
  indices(std::move(other.indices)),
  vbo(std::move(other.vbo)),
  layout(std::move(other.layout)),
  vao(std::move(other.vao)),
  ibo(std::move(other.ibo))
{
}

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices)
{
	this->vertices = vertices;
	this->indices = indices;

	vbo.Init(this->vertices.data(), sizeof(GLfloat) * this->vertices.size() * 8);

	this->layout.Push<GLfloat>(3);
	this->layout.Push<GLfloat>(3);
	this->layout.Push<GLfloat>(2);

	vao.AddBuffer(vbo, layout);

	ibo.Init(this->indices.data(), this->indices.size());
}

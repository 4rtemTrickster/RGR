#include "Renderer.h"

void Renderer::Clear()const
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const GL::VertexArray& va, const GL::IndexBuffer& ib, const GL::Shader& shader) const
{
	shader.Bind();

	va.Bind();
	ib.Bind();

	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

#include "Renderer.h"

void Renderer::Clear()const
{
	GLCall(glClearColor(0.2f, 0.5f, 0.1f, 1.0f));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::Draw(const GL::VertexArray& va, const GL::IndexBuffer& ib, const GL::Shader& shader) const
{
	shader.Bind();

	va.Bind();
	ib.Bind();

	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

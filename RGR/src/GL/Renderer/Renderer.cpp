#include "Help.h"
#include "Renderer.h"

void GL::Renderer::Clear(glm::vec4 ClearColor, GLbitfield mask)const
{
	GLCall(glClearColor(ClearColor.r, ClearColor.g, ClearColor.b, ClearColor.a));
	GLCall(glClear(mask));
}

void GL::Renderer::Draw(const GL::VertexArray& va, const GL::IndexBuffer& ib, const GL::Shader& shader) const
{
	shader.Bind();

	va.Bind();
	ib.Bind();

	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

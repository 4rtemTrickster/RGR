#include "VertexBuffer.h"

GL::VertexBuffer::VertexBuffer(const void* data, GLuint size)
{
	GLCall(glGenBuffers(1, &m_RendererID))
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID))
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW))
}

GL::VertexBuffer::~VertexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_RendererID))
}

void GL::VertexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID))
}

void GL::VertexBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0))
}
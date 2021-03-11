#include "IndexBuffer.h"

GL::IndexBuffer::IndexBuffer(const GLuint* data, GLuint count)
	: m_Count(count)
{
	GLCall(glGenBuffers(1, &m_RendererID))
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID))
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW))
}

GL::IndexBuffer::~IndexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_RendererID))
}

void GL::IndexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID))
}

void GL::IndexBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0))
}

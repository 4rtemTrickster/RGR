#include "Help.h"
#include "VertexBuffer.h"

GL::VertexBuffer::VertexBuffer()
	: InitializationFlag(false)
{
}

GL::VertexBuffer::VertexBuffer(const void* data, GLuint size)
{
	GLCall(glGenBuffers(1, &m_RendererID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));

	InitializationFlag = true;
}

GL::VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
	: m_RendererID(std::move(other.m_RendererID)),
	InitializationFlag(true)
{
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

void GL::VertexBuffer::Init(const void* data, GLuint size)
{
	if (!InitializationFlag)
	{
		GLCall(glGenBuffers(1, &m_RendererID));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));

		InitializationFlag = true;
	}
}
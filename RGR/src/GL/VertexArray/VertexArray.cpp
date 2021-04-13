#include "Help.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"

GL::VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RendererID));
}

GL::VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void GL::VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();

	const auto& elements = layout.GetElements();
	GLuint offset = 0;

	for (GLuint i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));

		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
		
	}
}

void GL::VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RendererID));
}

void GL::VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}

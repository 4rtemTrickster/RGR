#pragma once
#include "../VertexBuffer/VertexBuffer.h"


namespace GL
{
	class VertexBufferLayout;
	
	class VertexArray
	{
	private:
		GLuint m_RendererID;
	public:
		VertexArray();
		~VertexArray();

		void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

		void Bind() const;
		void Unbind() const;
	};
}


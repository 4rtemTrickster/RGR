#pragma once
#include "Help.h"

namespace GL
{
	class IndexBuffer
	{
	private:
		GLuint m_RendererID;
		GLuint m_Count;

		bool InitializationFlag;

	public:
		IndexBuffer();
		IndexBuffer(const GLuint* data, GLuint count);
		~IndexBuffer();

		void Bind() const;
		void Unbind() const;

		inline GLuint GetCount() const { return m_Count; }

		inline bool IsInit() const { return InitializationFlag; }
		void Init(const GLuint* data, GLuint count);
	};
}


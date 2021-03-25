#pragma once
#include "../../Help.h"

namespace GL
{
	class VertexBuffer
	{
	private:
		GLuint m_RendererID;

		bool InitializationFlag;

	public:
		VertexBuffer();
		VertexBuffer(const void* data, GLuint size);
		~VertexBuffer();

		void Bind() const;
		void Unbind() const;

		inline bool IsInit() const { return InitializationFlag; }
		void Init(const void* data, GLuint size);
	};
}
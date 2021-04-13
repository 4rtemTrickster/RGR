#pragma once
#include "Help.h"
#include "GL/VertexArray/VertexArray.h"
#include "GL/IndexBuffer/IndexBuffer.h"
#include "GL/Shader/Shader.h"

namespace GL
{
	class Renderer
	{
	public:
		void Clear(glm::vec4 ClearColor, GLbitfield mask) const;
	
		void Draw(const GL::VertexArray& va, const GL::IndexBuffer& ib, const GL::Shader& shader) const;
	};
}

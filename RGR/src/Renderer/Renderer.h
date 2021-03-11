#pragma once
#include "../Help.h"
#include "../GL/VertexArray/VertexArray.h"
#include "../GL/IndexBuffer/IndexBuffer.h"
#include "../GL/Shader/Shader.h"

class Renderer
{
public:
	void Clear() const;
	
	void Draw(const GL::VertexArray& va, const GL::IndexBuffer& ib, const GL::Shader& shader) const;
};

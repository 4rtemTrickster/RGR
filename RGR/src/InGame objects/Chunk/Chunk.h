#pragma once

#include "../../Help.h"
#include "../../GL/Renderer/Renderer.h"
#include "../../GL/Texture/Texture.h"
#include "../../GL/VertexArray/VertexBufferLayout.h"

class Chunk
{
private:

	

public:
	//Chunk params
	static const GLuint Chunk_Width = 16;
	static const GLuint Chunk_Length = 16;
	static const GLuint Chunk_Height = 256;

	Chunk();
	~Chunk();
};

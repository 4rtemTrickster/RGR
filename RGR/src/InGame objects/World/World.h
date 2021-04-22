#pragma once

#include "Chunk/Chunk.h"

class World
{
public:
	static const GLuint World_Width = 32;
	static const GLuint World_Length = 32;
	static const GLuint World_Height = 32;
	static const GLuint World_Volume = World_Width * World_Length * World_Height;

	std::vector<Mesh*> GenerateMeshes();
	std::pair<std::vector<Vertex>,std::vector<GLuint>> ProcessBlock(
		const GLuint XStart = 0,
		const GLuint YStart = 0,
		const GLuint ZStart = 0,
		
		const GLuint XBorder = World_Width,
		const GLuint YBorder = World_Height,
		const GLuint ZBorder = World_Length);
	std::vector<Chunk*> Chunks = {nullptr};

	World();
	~World();
};

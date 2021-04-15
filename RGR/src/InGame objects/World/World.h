#pragma once

#include "Chunk/Chunk.h"

class World
{
public:
	static const GLuint World_Width = 16;
	static const GLuint World_Length = 16;
	static const GLuint World_Volume = World_Width * World_Length;

	std::vector<Mesh*> GenerateMesh();
	std::vector<Chunk*> Chunks;

	World();
	~World();
};

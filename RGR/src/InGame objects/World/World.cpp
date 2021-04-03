#include "World.h"

World::World()
{
	for (size_t i = 0; i < World_Length; i++)
	{
		for (size_t j = 0; j < World_Width; j++)
		{
			Chunks[i][j].Init(i * Chunk::Chunk_Width, j * Chunk::Chunk_Length);
		}
	}
}

World::~World()
{
}

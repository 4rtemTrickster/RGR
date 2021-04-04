#include "World.h"

World::World()
{
    Chunks.reserve(World_Volume);

    for (int z = 0; z < World_Length; ++z)
        for (int x = 0; x < World_Width; ++x)
            Chunks.push_back(new Chunk(x * World_Width, z * World_Length));

    Chunks.shrink_to_fit();
}

World::~World()
{
}

#include "Help.h"
#include "Chunk.h"


Chunk::Chunk(GLint WorldX, GLint WorldZ)
    : WorldX(WorldX), WorldZ(WorldZ)
{
    this->voxels = new Voxel[Chunk_Volume];

    for (size_t z = 0; z < Chunk_Length; z++)
    {
        for (size_t x = 0; x < Chunk_Width; x++)
        {
            const GLfloat height = glm::perlin(glm::vec3((x + WorldX) * 0.025f, (z + WorldZ) * 0.025f, 0.0f)) + 1;
            for (size_t y = 0; y < Chunk_Height; y++)
            {
                GLint id = y <= height * 25;

                if (y < height * 25 - 5)
                    id = 2;

                this->voxels[(y * Chunk_Length + z) * Chunk_Width + x].id = id;
            }
        }
    }
}

Chunk::~Chunk()
{
    delete[] this->voxels;
}
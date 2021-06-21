#include "Help.h"
#include "Chunk.h"


Chunk::Chunk(GLint WorldX, GLint WorldZ)
    : WorldX(WorldX), WorldZ(WorldZ)
{
    voxels = new Voxel[Chunk_Volume];

    for (size_t z = 0; z < Chunk_Length; z++)
    {
        for (size_t x = 0; x < Chunk_Width; x++)
        {
            const GLfloat height = glm::perlin(glm::vec3((x + WorldX) * 0.02f, (z + WorldZ) * 0.02f, 0.0f)) + 2;

            for (size_t y = 0; y < Chunk_Height; y++)
            {
                GLint id = y <= height * 25 + 10;

                if (y < height * 25 + 10 - 1)
                    id = 2;
                if (y < height * 25 + 10 - 5)
                    id = 3;
                if (y == 0)
                    id = 4;

                voxels[(y * Chunk_Length + z) * Chunk_Width + x].id = id;
            }
        }
    }
}

Chunk::~Chunk()
{
}

Voxel* Chunk::GetVoxelByGlobalCoord()
{
    
}

#include "Chunk.h"

#include "../../Mesh/Mesh.h"
#include "glm/glm.hpp"
#include "glm/gtc/noise.hpp"

Chunk::Chunk(GLint WorldX, GLint WorldZ)
    : WorldX(WorldX), WorldZ(WorldZ)
{
    this->voxels = new Voxel[Chunk_Volume];

    for (size_t y = 0; y < Chunk_Height; y++)
    {
        for (size_t z = 0; z < Chunk_Length; z++)
        {
            for (size_t x = 0; x < Chunk_Width; x++)
            {
                this->voxels[(y * Chunk_Length + z) * Chunk_Width + x].id = y <= (std::sin((x + WorldX) * 0.1f) * 0.5f + 0.5f) * 50 && y <= (std::sin((z + WorldZ) * 0.1f) * 0.5f + 0.5f) * 50;
                //VOXEL(x,y,z).id = glm::perlin(glm::vec3((x+WorldX) * 0.0125f, y * 0.0125f, (z+WorldZ)*0.0125f)) > 0.1f;
            }
        }
    }

}

Chunk::~Chunk()
{
    delete[] this->voxels;
}
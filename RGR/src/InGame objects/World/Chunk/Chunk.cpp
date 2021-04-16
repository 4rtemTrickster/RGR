#include "Help.h"
#include "Chunk.h"


Chunk::Chunk(GLint WorldX, GLint WorldZ)
    : WorldX(WorldX), WorldZ(WorldZ)
{
    this->voxels = new Voxel[Chunk_Volume];

    // for (size_t y = 0; y < Chunk_Height; y++)
    // {
    //     for (size_t z = 0; z < Chunk_Length; z++)
    //     {
    //         for (size_t x = 0; x < Chunk_Width; x++)
    //         {
    //             //const GLuint id = y <= (std::sin((x + WorldX) * 0.1f) * 0.5f + 0.5f) * 50 && y <= (std::sin((z + WorldZ) * 0.1f) * 0.5f + 0.5f) * 50;
    //             const GLuint id = glm::perlin(glm::vec3((x+WorldX) * 0.0125f, y * 0.0125f, (z+WorldZ)*0.0125f)) > 0.1f;
    //             
    //             if (id && y <=2)
    //                 this->voxels[(y * Chunk_Length + z) * Chunk_Width + x].id = 2;
    //             else
    //                 this->voxels[(y * Chunk_Length + z) * Chunk_Width + x].id = id;
    //             
    //             //this->voxels[(y * Chunk_Length + z) * Chunk_Width + x].id = 1;
    //         }
    //     }
    // }

    for (size_t z = 0; z < Chunk_Length; z++)
    {
        for (size_t x = 0; x < Chunk_Width; x++)
        {
            const GLfloat height = glm::perlin(glm::vec3((x + WorldX) * 0.025f, (z + WorldZ) * 0.025f, 0.0f)) + 1;
            for (size_t y = 0; y < Chunk_Height; y++)
            {
                GLint id = y <= (height) * 25;

                if (y == 0)
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
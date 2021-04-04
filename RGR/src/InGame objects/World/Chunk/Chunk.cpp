#include "Chunk.h"

#include "../../Mesh/Mesh.h"
#include "glm/glm.hpp"

/**
 * \brief Check's for the presence of a voxel in the chunk
 */
#define IS_IN_CHUNK(X,Y,Z) (((X) >= 0 && (X) < Chunk_Width) && ((Y) >= 0 && (Y) < Chunk_Height) && ((Z) >= 0 && (Z) < Chunk_Length))

/**
 * \brief Provides easier access to the voxel
 */
#define VOXEL(X,Y,Z) (this->voxels[((Y) * Chunk_Length + (Z)) * Chunk_Width + (X)])

/**
 * \brief Checks whether the voxel is being drawn
 */
#define IS_TO_DRAW(X,Y,Z) ((IS_IN_CHUNK(X,Y,Z)) && (VOXEL(X,Y,Z).id))

/**
* \brief Provides an easier way to push vertex data into an array
* \param X X coordinate of the vertex
* \param Y Y coordinate of the vertex
* \param Z Z coordinate of the vertex
* \param NX X coordinate of the normal vector
* \param NY Y coordinate of the normal vector
* \param NZ Z coordinate of the normal vector
* \param TX Texture coordinate X
* \param TY Texture coordinate Y
*/
#define PUSH_BACK_VERTEX(X,Y,Z, NX,NY,NZ, TX,TY) \
	this->vertices.push_back({glm::vec3(X,Y,Z), glm::vec3(NX,NY,NZ), glm::vec2(TX,TY)});


Chunk::Chunk(const Chunk& other)
    : WorldX(other.WorldX),
      WorldZ(other.WorldZ),
      vertices(other.vertices),
      indices(other.indices)
{
}

Chunk::Chunk(Chunk&& other) noexcept
    : WorldX(other.WorldX),
      WorldZ(other.WorldZ),
      vertices(std::move(other.vertices)),
      indices(std::move(other.indices))
{
}

Chunk& Chunk::operator=(const Chunk& other)
{
    if (this == &other)
        return *this;
    WorldX = other.WorldX;
    WorldZ = other.WorldZ;
    vertices = other.vertices;
    indices = other.indices;
    return *this;
}

Chunk& Chunk::operator=(Chunk&& other) noexcept
{
    if (this == &other)
        return *this;
    WorldX = other.WorldX;
    WorldZ = other.WorldZ;
    vertices = std::move(other.vertices);
    indices = std::move(other.indices);
    return *this;
}

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
                VOXEL(x, y, z).id = y <= (std::sin((x + WorldX) * 0.1f) * 0.5f + 0.5f) * 50 && y <= (std::sin((z + WorldZ) * 0.1f) * 0.5f + 0.5f) * 50;
            }
        }
    }
}

Chunk::~Chunk()
{
    delete[] this->voxels;
}

Mesh* Chunk::GenerateMesh()
{
    size_t index = 0;

    vertices.reserve(Chunk_Volume * 6);
    indices.reserve(Chunk_Volume * 6);

    const GLfloat uvsize = 1.0f / 16.0f;

    for (size_t y = 0; y < Chunk_Height; y++)
    {
        for (size_t z = 0; z < Chunk_Length; z++)
        {
            for (size_t x = 0; x < Chunk_Width; x++)
            {
                GLuint id = VOXEL(x, y, z).id;
                
                if (!id)
                    continue;

                float u = (id % 16) * uvsize;
                float v = 1-((1 + id / 16) * uvsize);

                // Front to def camera position
                if (!IS_TO_DRAW(x, y, z + 1))
                {
                    indices.push_back(index++);
                    PUSH_BACK_VERTEX(x - 0.5f + WorldX, y - 0.5f, z + 0.5f + WorldZ, 0.0f, 0.0f, 1.0f, u*2, v);

                    indices.push_back(index++);
                    PUSH_BACK_VERTEX(x + 0.5f + WorldX, y - 0.5f, z + 0.5f + WorldZ, 0.0f, 0.0f, 1.0f, u*2+uvsize, v);

                    indices.push_back(index);
                    indices.push_back(index++);
                    PUSH_BACK_VERTEX(x + 0.5f + WorldX, y + 0.5f, z + 0.5f + WorldZ, 0.0f, 0.0f, 1.0f, u*2 + uvsize, v + uvsize);

                    indices.push_back(index++);
                    PUSH_BACK_VERTEX(x - 0.5f + WorldX, y + 0.5f, z + 0.5f + WorldZ, 0.0f, 0.0f, 1.0f, u*2,v + uvsize);

                    indices.push_back(index - 4);
                }

                // Back
                if (!IS_TO_DRAW(x, y, z - 1))
                {
                    indices.push_back(index++);
                    PUSH_BACK_VERTEX(x + 0.5f + WorldX, y - 0.5f, z - 0.5f + WorldZ, 0.0f, 0.0f, -1.0f, u*2, v);

                    indices.push_back(index++);
                    PUSH_BACK_VERTEX(x - 0.5f + WorldX, y - 0.5f, z - 0.5f + WorldZ, 0.0f, 0.0f, -1.0f, u*2+uvsize, v);

                    indices.push_back(index);
                    indices.push_back(index++);
                    PUSH_BACK_VERTEX(x - 0.5f + WorldX, y + 0.5f, z - 0.5f + WorldZ, 0.0f, 0.0f, -1.0f, u*2 + uvsize, v + uvsize);

                    indices.push_back(index++);
                    PUSH_BACK_VERTEX(x + 0.5f + WorldX, y + 0.5f, z - 0.5f + WorldZ, 0.0f, 0.0f, -1.0f, u*2,v + uvsize);

                    indices.push_back(index - 4);
                }

                // Right
                if (!IS_TO_DRAW(x + 1, y, z))
                {
                    indices.push_back(index++);
                    PUSH_BACK_VERTEX(x + 0.5f + WorldX, y - 0.5f, z + 0.5f + WorldZ, 1.0f, 0.0f, 0.0f, u*2, v);

                    indices.push_back(index++);
                    PUSH_BACK_VERTEX(x + 0.5f + WorldX, y - 0.5f, z - 0.5f + WorldZ, 1.0f, 0.0f, 0.0f, u*2+uvsize, v);

                    indices.push_back(index);
                    indices.push_back(index++);
                    PUSH_BACK_VERTEX(x + 0.5f + WorldX, y + 0.5f, z - 0.5f + WorldZ, 1.0f, 0.0f, 0.0f, u*2 + uvsize, v + uvsize);

                    indices.push_back(index++);
                    PUSH_BACK_VERTEX(x + 0.5f + WorldX, y + 0.5f, z + 0.5f + WorldZ, 1.0f, 0.0f, 0.0f, u*2,v + uvsize);

                    indices.push_back(index - 4);
                }

                // Left
                if (!IS_TO_DRAW(x - 1, y, z))
                {
                    indices.push_back(index++);
                    PUSH_BACK_VERTEX(x - 0.5f + WorldX, y - 0.5f, z - 0.5f + WorldZ, -1.0f, 0.0f, 0.0f, u*2, v);

                    indices.push_back(index++);
                    PUSH_BACK_VERTEX(x - 0.5f + WorldX, y - 0.5f, z + 0.5f + WorldZ, -1.0f, 0.0f, 0.0f, u*2+uvsize, v);

                    indices.push_back(index);
                    indices.push_back(index++);
                    PUSH_BACK_VERTEX(x - 0.5f + WorldX, y + 0.5f, z + 0.5f + WorldZ, -1.0f, 0.0f, 0.0f, u*2 + uvsize, v + uvsize);

                    indices.push_back(index++);
                    PUSH_BACK_VERTEX(x - 0.5f + WorldX, y + 0.5f, z - 0.5f + WorldZ, -1.0f, 0.0f, 0.0f, u*2,v + uvsize);

                    indices.push_back(index - 4);
                }

                // Top
                if (!IS_TO_DRAW(x, y + 1, z))
                {
                    indices.push_back(index++);
                    PUSH_BACK_VERTEX(x - 0.5f + WorldX, y + 0.5f, z + 0.5f + WorldZ, 0.0f, 1.0f, 0.0f, u, v);

                    indices.push_back(index++);
                    PUSH_BACK_VERTEX(x + 0.5f + WorldX, y + 0.5f, z + 0.5f + WorldZ, 0.0f, 1.0f, 0.0f, u+uvsize, v);

                    indices.push_back(index);
                    indices.push_back(index++);
                    PUSH_BACK_VERTEX(x + 0.5f + WorldX, y + 0.5f, z - 0.5f + WorldZ, 0.0f, 1.0f, 0.0f, u + uvsize, v + uvsize);

                    indices.push_back(index++);
                    PUSH_BACK_VERTEX(x - 0.5f + WorldX, y + 0.5f, z - 0.5f + WorldZ, 0.0f, 1.0f, 0.0f, u,v + uvsize);

                    indices.push_back(index - 4);
                }

                // Bottom
                if (!IS_TO_DRAW(x, y - 1, z))
                {
                    indices.push_back(index++);
                    PUSH_BACK_VERTEX(x - 0.5f + WorldX, y - 0.5f, z - 0.5f + WorldZ, 0.0f, -1.0f, 0.0f, u*3, v);

                    indices.push_back(index++);
                    PUSH_BACK_VERTEX(x + 0.5f + WorldX, y - 0.5f, z - 0.5f + WorldZ, 0.0f, -1.0f, 0.0f, u*3+uvsize, v);

                    indices.push_back(index);
                    indices.push_back(index++);
                    PUSH_BACK_VERTEX(x + 0.5f + WorldX, y - 0.5f, z + 0.5f + WorldZ, 0.0f, -1.0f, 0.0f, u*3 + uvsize, v + uvsize);

                    indices.push_back(index++);
                    PUSH_BACK_VERTEX(x - 0.5f + WorldX, y - 0.5f, z + 0.5f + WorldZ, 0.0f, -1.0f, 0.0f, u*3,v + uvsize);

                    indices.push_back(index - 4);
                }
            }
        }
    }

    vertices.shrink_to_fit();
    indices.shrink_to_fit();

    return new Mesh(vertices, indices);
}

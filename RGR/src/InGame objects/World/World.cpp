#include "World.h"

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
vertices.push_back({glm::vec3(X,Y,Z), glm::vec3(NX,NY,NZ), glm::vec2(TX,TY)});

World::World()
{
    Chunks.reserve(World_Volume);

    for (int z = 0; z < World_Length; ++z)
    {
        for (int x = 0; x < World_Width; ++x)
        {
            Chunks.push_back(new Chunk(x * World_Width, z * World_Length));
        }
    }
    
    Chunks.shrink_to_fit();
}

Mesh* World::GenerateMesh()
{
    size_t index = 0;
    const GLfloat uvsize = 1.0f / 16.0f;
    const GLfloat SideShift = 1 * uvsize;
    const GLfloat BottomShift = 2 * uvsize;
    const GLuint AtlasShift = 3;

    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    vertices.reserve(Chunk::Chunk_Volume * 6);
    indices.reserve(Chunk::Chunk_Volume * 6);

    
    Chunk* current = this->Chunks[0];

    for (int i = 0; i < World_Volume; ++i)
    {
        for (size_t y = 0; y < Chunk::Chunk_Height; y++)
        {
            for (size_t z = 0; z < Chunk::Chunk_Length; z++)
            {
                for (size_t x = 0; x < Chunk::Chunk_Width; x++)
                {
                    GLuint id = VOXEL(x, y, z).id;
                
                    if (!id)
                        continue;

                    float u = (id * AtlasShift % 16) * uvsize;
                    float v = 1-((1 + id / 16) * uvsize);

                    // Front to def camera position
                    if (!IS_TO_DRAW(x, y, z + 1))
                    {
                        indices.push_back(index++);
                        PUSH_BACK_VERTEX(x - 0.5f + WorldX, y - 0.5f, z + 0.5f + WorldZ, 0.0f, 0.0f, 1.0f, u + SideShift, v);

                        indices.push_back(index++);
                        PUSH_BACK_VERTEX(x + 0.5f + WorldX, y - 0.5f, z + 0.5f + WorldZ, 0.0f, 0.0f, 1.0f, u + SideShift+uvsize, v);

                        indices.push_back(index);
                        indices.push_back(index++);
                        PUSH_BACK_VERTEX(x + 0.5f + WorldX, y + 0.5f, z + 0.5f + WorldZ, 0.0f, 0.0f, 1.0f, u + SideShift + uvsize, v + uvsize);

                        indices.push_back(index++);
                        PUSH_BACK_VERTEX(x - 0.5f + WorldX, y + 0.5f, z + 0.5f + WorldZ, 0.0f, 0.0f, 1.0f, u + SideShift,v + uvsize);

                        indices.push_back(index - 4);
                    }

                    // Back
                    if (!IS_TO_DRAW(x, y, z - 1))
                    {
                        indices.push_back(index++);
                        PUSH_BACK_VERTEX(x + 0.5f + WorldX, y - 0.5f, z - 0.5f + WorldZ, 0.0f, 0.0f, -1.0f, u + SideShift, v);

                        indices.push_back(index++);
                        PUSH_BACK_VERTEX(x - 0.5f + WorldX, y - 0.5f, z - 0.5f + WorldZ, 0.0f, 0.0f, -1.0f, u + SideShift+uvsize, v);

                        indices.push_back(index);
                        indices.push_back(index++);
                        PUSH_BACK_VERTEX(x - 0.5f + WorldX, y + 0.5f, z - 0.5f + WorldZ, 0.0f, 0.0f, -1.0f, u + SideShift + uvsize, v + uvsize);

                        indices.push_back(index++);
                        PUSH_BACK_VERTEX(x + 0.5f + WorldX, y + 0.5f, z - 0.5f + WorldZ, 0.0f, 0.0f, -1.0f, u + SideShift,v + uvsize);

                        indices.push_back(index - 4);
                    }

                    // Right
                    if (!IS_TO_DRAW(x + 1, y, z))
                    {
                        indices.push_back(index++);
                        PUSH_BACK_VERTEX(x + 0.5f + WorldX, y - 0.5f, z + 0.5f + WorldZ, 1.0f, 0.0f, 0.0f, u + SideShift, v);

                        indices.push_back(index++);
                        PUSH_BACK_VERTEX(x + 0.5f + WorldX, y - 0.5f, z - 0.5f + WorldZ, 1.0f, 0.0f, 0.0f, u + SideShift+uvsize, v);

                        indices.push_back(index);
                        indices.push_back(index++);
                        PUSH_BACK_VERTEX(x + 0.5f + WorldX, y + 0.5f, z - 0.5f + WorldZ, 1.0f, 0.0f, 0.0f, u + SideShift + uvsize, v + uvsize);

                        indices.push_back(index++);
                        PUSH_BACK_VERTEX(x + 0.5f + WorldX, y + 0.5f, z + 0.5f + WorldZ, 1.0f, 0.0f, 0.0f, u + SideShift,v + uvsize);

                        indices.push_back(index - 4);
                    }

                    // Left
                    if (!IS_TO_DRAW(x - 1, y, z))
                    {
                        indices.push_back(index++);
                        PUSH_BACK_VERTEX(x - 0.5f + WorldX, y - 0.5f, z - 0.5f + WorldZ, -1.0f, 0.0f, 0.0f, u + SideShift, v);

                        indices.push_back(index++);
                        PUSH_BACK_VERTEX(x - 0.5f + WorldX, y - 0.5f, z + 0.5f + WorldZ, -1.0f, 0.0f, 0.0f, u + SideShift+uvsize, v);

                        indices.push_back(index);
                        indices.push_back(index++);
                        PUSH_BACK_VERTEX(x - 0.5f + WorldX, y + 0.5f, z + 0.5f + WorldZ, -1.0f, 0.0f, 0.0f, u + SideShift + uvsize, v + uvsize);

                        indices.push_back(index++);
                        PUSH_BACK_VERTEX(x - 0.5f + WorldX, y + 0.5f, z - 0.5f + WorldZ, -1.0f, 0.0f, 0.0f, u + SideShift, v + uvsize);

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
                        PUSH_BACK_VERTEX(x - 0.5f + WorldX, y - 0.5f, z - 0.5f + WorldZ, 0.0f, -1.0f, 0.0f, u + BottomShift, v);

                        indices.push_back(index++);
                        PUSH_BACK_VERTEX(x + 0.5f + WorldX, y - 0.5f, z - 0.5f + WorldZ, 0.0f, -1.0f, 0.0f, u + BottomShift+uvsize, v);

                        indices.push_back(index);
                        indices.push_back(index++);
                        PUSH_BACK_VERTEX(x + 0.5f + WorldX, y - 0.5f, z + 0.5f + WorldZ, 0.0f, -1.0f, 0.0f, u + BottomShift + uvsize, v + uvsize);

                        indices.push_back(index++);
                        PUSH_BACK_VERTEX(x - 0.5f + WorldX, y - 0.5f, z + 0.5f + WorldZ, 0.0f, -1.0f, 0.0f, u + BottomShift,v + uvsize);

                        indices.push_back(index - 4);
                    }
                }
            }
        }
    }

    vertices.shrink_to_fit();
    indices.shrink_to_fit();

    return new Mesh(vertices, indices);
}

World::~World()
{
    for(auto var : Chunks)
        delete var;
}

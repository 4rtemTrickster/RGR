#include "Help.h"
#include "World.h"

#define CHUNK(X,Z) (this->Chunks[(X) * Chunk::Chunk_Length + (Z)])

/**
* \brief Check's for the presence of a voxel in the chunk
*/
#define IS_IN_CHUNK(X,Y,Z) (((X) >= 0 && (X) < Chunk::Chunk_Width) && ((Y) >= 0 && (Y) < Chunk::Chunk_Height) && ((Z) >= 0 && (Z) < Chunk::Chunk_Length))

#define CHUNK_IN_WORLD(X,Z) (((X) >=0 && (X) < World_Width) && ((Z) >= 0 && (Z) < World_Length))


/**
 * \brief Provides easier access to the Voxel
 * \param X X coordinate of the Voxel
 * \param Y Y coordinate of the Voxel
 * \param Z Z coordinate of the Voxel
 * \param CX X coordinate of the Chunk
 * \param CZ Z coordinate of the Chunk
 */
#define VOXEL(X,Y,Z, CX,CZ) (CHUNK(CX,CZ)->GetVoxels()[((Y) * Chunk::Chunk_Length + (Z)) * Chunk::Chunk_Width + (X)])

/**
* \brief Checks whether the voxel is being drawn
*/
#define IS_TO_DRAW(X,Y,Z, CX,CZ) ((IS_IN_CHUNK(X,Y,Z)) && (CHUNK_IN_WORLD(CX,CZ) && (VOXEL(X,Y,Z, CX,CZ).id)))

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
    GLdouble start_time = glfwGetTime();
    
    Chunks.reserve(World_Volume);

    for (int z = 0; z < World_Length; ++z)
    {
        for (int x = 0; x < World_Width; ++x)
        {
            Chunks.push_back(new Chunk(x * World_Width, z * World_Length));
        }
    }
    
    //Chunks.shrink_to_fit();

    LOG_INFO("World generated in {0} seconds", glfwGetTime() - start_time);
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

    vertices.reserve(Chunk::Chunk_Volume * World_Volume);
    indices.reserve(Chunk::Chunk_Volume * World_Volume);


    for (size_t CZ = 0; CZ < World_Length; ++CZ)
    {
        for (size_t CX = 0; CX < World_Width; ++CX)
        {
            for (size_t y = 0; y < Chunk::Chunk_Height; y++)
            {
                for (size_t z = 0; z < Chunk::Chunk_Length; z++)
                {
                    for (size_t x = 0; x < Chunk::Chunk_Width; x++)
                    {
                        const GLuint id = VOXEL(x, y, z, CX, CZ).id;
                
                        if (!id)
                            continue;

                        float u = (id * AtlasShift % 16) * uvsize;
                        float v = 1-((1 + id / 16) * uvsize);

                        // Front to def camera position
                        if (!IS_TO_DRAW(x, y, z + 1, CX, CZ))
                        {
                            indices.push_back(index++);
                            PUSH_BACK_VERTEX(x - 0.5f + CHUNK(CX,CZ)->GetWorldX(), y - 0.5f, z + 0.5f + CHUNK(CX,CZ)->GetWorldZ(), 0.0f, 0.0f, 1.0f, u + SideShift, v);

                            indices.push_back(index++);
                            PUSH_BACK_VERTEX(x + 0.5f + CHUNK(CX,CZ)->GetWorldX(), y - 0.5f, z + 0.5f + CHUNK(CX,CZ)->GetWorldZ(), 0.0f, 0.0f, 1.0f, u + SideShift+uvsize, v);

                            indices.push_back(index);
                            indices.push_back(index++);
                            PUSH_BACK_VERTEX(x + 0.5f + CHUNK(CX,CZ)->GetWorldX(), y + 0.5f, z + 0.5f + CHUNK(CX,CZ)->GetWorldZ(), 0.0f, 0.0f, 1.0f, u + SideShift + uvsize, v + uvsize);

                            indices.push_back(index++);
                            PUSH_BACK_VERTEX(x - 0.5f + CHUNK(CX,CZ)->GetWorldX(), y + 0.5f, z + 0.5f + CHUNK(CX,CZ)->GetWorldZ(), 0.0f, 0.0f, 1.0f, u + SideShift,v + uvsize);

                            indices.push_back(index - 4);
                        }

                        // Back
                        if (!IS_TO_DRAW(x, y, z - 1, CX, CZ))
                        {
                            indices.push_back(index++);
                            PUSH_BACK_VERTEX(x + 0.5f + CHUNK(CX,CZ)->GetWorldX(), y - 0.5f, z - 0.5f + CHUNK(CX,CZ)->GetWorldZ(), 0.0f, 0.0f, -1.0f, u + SideShift, v);

                            indices.push_back(index++);
                            PUSH_BACK_VERTEX(x - 0.5f + CHUNK(CX,CZ)->GetWorldX(), y - 0.5f, z - 0.5f + CHUNK(CX,CZ)->GetWorldZ(), 0.0f, 0.0f, -1.0f, u + SideShift+uvsize, v);

                            indices.push_back(index);
                            indices.push_back(index++);
                            PUSH_BACK_VERTEX(x - 0.5f + CHUNK(CX,CZ)->GetWorldX(), y + 0.5f, z - 0.5f + CHUNK(CX,CZ)->GetWorldZ(), 0.0f, 0.0f, -1.0f, u + SideShift + uvsize, v + uvsize);

                            indices.push_back(index++);
                            PUSH_BACK_VERTEX(x + 0.5f + CHUNK(CX,CZ)->GetWorldX(), y + 0.5f, z - 0.5f + CHUNK(CX,CZ)->GetWorldZ(), 0.0f, 0.0f, -1.0f, u + SideShift,v + uvsize);

                            indices.push_back(index - 4);
                        }

                        // Right
                        if (!IS_TO_DRAW(x + 1, y, z, CX, CZ))
                        {
                            indices.push_back(index++);
                            PUSH_BACK_VERTEX(x + 0.5f + CHUNK(CX,CZ)->GetWorldX(), y - 0.5f, z + 0.5f + CHUNK(CX,CZ)->GetWorldZ(), 1.0f, 0.0f, 0.0f, u + SideShift, v);

                            indices.push_back(index++);
                            PUSH_BACK_VERTEX(x + 0.5f + CHUNK(CX,CZ)->GetWorldX(), y - 0.5f, z - 0.5f + CHUNK(CX,CZ)->GetWorldZ(), 1.0f, 0.0f, 0.0f, u + SideShift+uvsize, v);

                            indices.push_back(index);
                            indices.push_back(index++);
                            PUSH_BACK_VERTEX(x + 0.5f + CHUNK(CX,CZ)->GetWorldX(), y + 0.5f, z - 0.5f + CHUNK(CX,CZ)->GetWorldZ(), 1.0f, 0.0f, 0.0f, u + SideShift + uvsize, v + uvsize);

                            indices.push_back(index++);
                            PUSH_BACK_VERTEX(x + 0.5f + CHUNK(CX,CZ)->GetWorldX(), y + 0.5f, z + 0.5f + CHUNK(CX,CZ)->GetWorldZ(), 1.0f, 0.0f, 0.0f, u + SideShift,v + uvsize);

                            indices.push_back(index - 4);
                        }

                        // Left
                        if (!IS_TO_DRAW(x - 1, y, z, CX, CZ))
                        {
                            indices.push_back(index++);
                            PUSH_BACK_VERTEX(x - 0.5f + CHUNK(CX,CZ)->GetWorldX(), y - 0.5f, z - 0.5f + CHUNK(CX,CZ)->GetWorldZ(), -1.0f, 0.0f, 0.0f, u + SideShift, v);

                            indices.push_back(index++);
                            PUSH_BACK_VERTEX(x - 0.5f + CHUNK(CX,CZ)->GetWorldX(), y - 0.5f, z + 0.5f + CHUNK(CX,CZ)->GetWorldZ(), -1.0f, 0.0f, 0.0f, u + SideShift+uvsize, v);

                            indices.push_back(index);
                            indices.push_back(index++);
                            PUSH_BACK_VERTEX(x - 0.5f + CHUNK(CX,CZ)->GetWorldX(), y + 0.5f, z + 0.5f + CHUNK(CX,CZ)->GetWorldZ(), -1.0f, 0.0f, 0.0f, u + SideShift + uvsize, v + uvsize);

                            indices.push_back(index++);
                            PUSH_BACK_VERTEX(x - 0.5f + CHUNK(CX,CZ)->GetWorldX(), y + 0.5f, z - 0.5f + CHUNK(CX,CZ)->GetWorldZ(), -1.0f, 0.0f, 0.0f, u + SideShift, v + uvsize);

                            indices.push_back(index - 4);
                        }

                        // Top
                        if (!IS_TO_DRAW(x, y + 1, z, CX, CZ))
                        {
                            indices.push_back(index++);
                            PUSH_BACK_VERTEX(x - 0.5f + CHUNK(CX,CZ)->GetWorldX(), y + 0.5f, z + 0.5f + CHUNK(CX,CZ)->GetWorldZ(), 0.0f, 1.0f, 0.0f, u, v);

                            indices.push_back(index++);
                            PUSH_BACK_VERTEX(x + 0.5f + CHUNK(CX,CZ)->GetWorldX(), y + 0.5f, z + 0.5f + CHUNK(CX,CZ)->GetWorldZ(), 0.0f, 1.0f, 0.0f, u+uvsize, v);

                            indices.push_back(index);
                            indices.push_back(index++);
                            PUSH_BACK_VERTEX(x + 0.5f + CHUNK(CX,CZ)->GetWorldX(), y + 0.5f, z - 0.5f + CHUNK(CX,CZ)->GetWorldZ(), 0.0f, 1.0f, 0.0f, u + uvsize, v + uvsize);

                            indices.push_back(index++);
                            PUSH_BACK_VERTEX(x - 0.5f + CHUNK(CX,CZ)->GetWorldX(), y + 0.5f, z - 0.5f + CHUNK(CX,CZ)->GetWorldZ(), 0.0f, 1.0f, 0.0f, u,v + uvsize);

                            indices.push_back(index - 4);
                        }

                        // Bottom
                        if (!IS_TO_DRAW(x, y - 1, z, CX, CZ))
                        {
                            indices.push_back(index++);
                            PUSH_BACK_VERTEX(x - 0.5f + CHUNK(CX,CZ)->GetWorldX(), y - 0.5f, z - 0.5f + CHUNK(CX,CZ)->GetWorldZ(), 0.0f, -1.0f, 0.0f, u + BottomShift, v);

                            indices.push_back(index++);
                            PUSH_BACK_VERTEX(x + 0.5f + CHUNK(CX,CZ)->GetWorldX(), y - 0.5f, z - 0.5f + CHUNK(CX,CZ)->GetWorldZ(), 0.0f, -1.0f, 0.0f, u + BottomShift+uvsize, v);

                            indices.push_back(index);
                            indices.push_back(index++);
                            PUSH_BACK_VERTEX(x + 0.5f + CHUNK(CX,CZ)->GetWorldX(), y - 0.5f, z + 0.5f + CHUNK(CX,CZ)->GetWorldZ(), 0.0f, -1.0f, 0.0f, u + BottomShift + uvsize, v + uvsize);

                            indices.push_back(index++);
                            PUSH_BACK_VERTEX(x - 0.5f + CHUNK(CX,CZ)->GetWorldX(), y - 0.5f, z + 0.5f + CHUNK(CX,CZ)->GetWorldZ(), 0.0f, -1.0f, 0.0f, u + BottomShift,v + uvsize);

                            indices.push_back(index - 4);
                        }
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

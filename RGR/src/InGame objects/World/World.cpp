#include "Help.h"
#include "World.h"

/**
 * \brief Provides easier access to the Voxel
 */
#define CHUNK(X,Z) (this->Chunks[((Z) * Chunk::Chunk_Width) + (X)])

/**
* \brief Check's for the presence of a voxel in the chunk
*/
#define IS_IN_CHUNK(X,Y,Z) (((X) >= 0 && (X) < Chunk::Chunk_Width) && ((Y) >= 0 && (Y) < Chunk::Chunk_Height) && ((Z) >= 0 && (Z) < Chunk::Chunk_Length))

/**
* \brief Check's for the presence of a Chunk in the World
*/
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
* \param X X coordinate of the Voxel
* \param Y Y coordinate of the Voxel
* \param Z Z coordinate of the Voxel
* \param CX X coordinate of the Chunk
* \param CZ Z coordinate of the Chunk
*/
#define IS_TO_DRAW(X,Y,Z, CX,CZ) ((IS_IN_CHUNK(X,Y,Z)) && CHUNK_IN_WORLD(CX,CZ) && (VOXEL(X,Y,Z, CX,CZ).id))

/**
* \brief Provides an easier way to push vertex data into an array
* \param Vector to push
* \param X X coordinate of the vertex
* \param Y Y coordinate of the vertex
* \param Z Z coordinate of the vertex
* \param NX X coordinate of the normal vector
* \param NY Y coordinate of the normal vector
* \param NZ Z coordinate of the normal vector
* \param TX Texture coordinate X
* \param TY Texture coordinate Y
*/
#define PUSH_BACK_VERTEX(VECTOR,X,Y,Z, NX,NY,NZ, TX,TY) \
VECTOR.push_back({glm::vec3(X,Y,Z), glm::vec3(NX,NY,NZ), glm::vec2(TX,TY)});

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
    
    Chunks.shrink_to_fit();

    LOG_INFO("World generated in {0} seconds", glfwGetTime() - start_time);
}

std::vector<Mesh*> World::GenerateMesh()
{
    const GLfloat uvsize = 1.0f / 16.0f;
    const GLfloat SideShift = 1 * uvsize;
    const GLfloat BottomShift = 2 * uvsize;
    const GLuint AtlasShift = 3;

    size_t FrontIndex = 0;
    size_t BackIndex = 0;
    size_t LeftIndex = 0;
    size_t RightIndex = 0;
    size_t TopIndex = 0;
    size_t BottomIndex = 0;

    std::vector<Vertex> FrontVertices;
    std::vector<GLuint> FrontIndices;

    std::vector<Vertex> BackVertices;
    std::vector<GLuint> BackIndices;

    std::vector<Vertex> LeftVertices;
    std::vector<GLuint> LeftIndices;

    std::vector<Vertex> RightVertices;
    std::vector<GLuint> RightIndices;

    std::vector<Vertex> TopVertices;
    std::vector<GLuint> TopIndices;

    std::vector<Vertex> BottomVertices;
    std::vector<GLuint> BottomIndices;

// #pragma omp parallel sections
//     {
// #pragma omp section
//         {
//             FrontVertices.reserve(Chunk::Chunk_Volume * World_Volume);
//             FrontIndices.reserve(Chunk::Chunk_Volume * World_Volume);
//         }
// #pragma omp section
//         {
//             BackVertices.reserve(Chunk::Chunk_Volume * World_Volume);
//             BackIndices.reserve(Chunk::Chunk_Volume * World_Volume);
//         }
// #pragma omp section
//         {
//             LeftVertices.reserve(Chunk::Chunk_Volume * World_Volume);
//             LeftIndices.reserve(Chunk::Chunk_Volume * World_Volume);
//         }
// #pragma omp section
//         {
//             RightVertices.reserve(Chunk::Chunk_Volume * World_Volume);
//             RightIndices.reserve(Chunk::Chunk_Volume * World_Volume);
//         }
// #pragma omp section
//         {
//             TopVertices.reserve(Chunk::Chunk_Volume * World_Volume);
//             TopIndices.reserve(Chunk::Chunk_Volume * World_Volume);
//         }
// #pragma omp section
//         {
//             BottomVertices.reserve(Chunk::Chunk_Volume * World_Volume);
//             BottomIndices.reserve(Chunk::Chunk_Volume * World_Volume);
//         }
//     }

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

#pragma omp parallel sections
                        {
#pragma omp section
                            {
                                // Front to def camera position
                                if (!IS_TO_DRAW(x, y, z + 1, CX, CZ) && !IS_TO_DRAW(x,y,z, CX, CZ + 1))
                                {
                                    FrontIndices.push_back(FrontIndex++);
                                    PUSH_BACK_VERTEX(FrontVertices,x - 0.5f + CHUNK(CX,CZ)->GetWorldX(), y - 0.5f, z + 0.5f + CHUNK(CX,CZ)->GetWorldZ(), 0.0f, 0.0f, 1.0f, u + SideShift, v);

                                    FrontIndices.push_back(FrontIndex++);
                                    PUSH_BACK_VERTEX(FrontVertices,x + 0.5f + CHUNK(CX,CZ)->GetWorldX(), y - 0.5f, z + 0.5f + CHUNK(CX,CZ)->GetWorldZ(), 0.0f, 0.0f, 1.0f, u + SideShift+uvsize, v);

                                    FrontIndices.push_back(FrontIndex);
                                    FrontIndices.push_back(FrontIndex++);
                                    PUSH_BACK_VERTEX(FrontVertices,x + 0.5f + CHUNK(CX,CZ)->GetWorldX(), y + 0.5f, z + 0.5f + CHUNK(CX,CZ)->GetWorldZ(), 0.0f, 0.0f, 1.0f, u + SideShift + uvsize, v + uvsize);

                                    FrontIndices.push_back(FrontIndex++);
                                    PUSH_BACK_VERTEX(FrontVertices,x - 0.5f + CHUNK(CX,CZ)->GetWorldX(), y + 0.5f, z + 0.5f + CHUNK(CX,CZ)->GetWorldZ(), 0.0f, 0.0f, 1.0f, u + SideShift,v + uvsize);

                                    FrontIndices.push_back(FrontIndex - 4);
                                }
                            }
#pragma omp section
                            {
                                // Back
                                if (!IS_TO_DRAW(x, y, z - 1, CX, CZ) && !IS_TO_DRAW(x,y,z, CX, CZ - 1))
                                {
                                    BackIndices.push_back(BackIndex++);
                                    PUSH_BACK_VERTEX(BackVertices,x + 0.5f + CHUNK(CX,CZ)->GetWorldX(), y - 0.5f, z - 0.5f + CHUNK(CX,CZ)->GetWorldZ(), 0.0f, 0.0f, -1.0f, u + SideShift, v);

                                    BackIndices.push_back(BackIndex++);
                                    PUSH_BACK_VERTEX(BackVertices,x - 0.5f + CHUNK(CX,CZ)->GetWorldX(), y - 0.5f, z - 0.5f + CHUNK(CX,CZ)->GetWorldZ(), 0.0f, 0.0f, -1.0f, u + SideShift+uvsize, v);

                                    BackIndices.push_back(BackIndex);
                                    BackIndices.push_back(BackIndex++);
                                    PUSH_BACK_VERTEX(BackVertices,x - 0.5f + CHUNK(CX,CZ)->GetWorldX(), y + 0.5f, z - 0.5f + CHUNK(CX,CZ)->GetWorldZ(), 0.0f, 0.0f, -1.0f, u + SideShift + uvsize, v + uvsize);

                                    BackIndices.push_back(BackIndex++);
                                    PUSH_BACK_VERTEX(BackVertices,x + 0.5f + CHUNK(CX,CZ)->GetWorldX(), y + 0.5f, z - 0.5f + CHUNK(CX,CZ)->GetWorldZ(), 0.0f, 0.0f, -1.0f, u + SideShift,v + uvsize);

                                    BackIndices.push_back(BackIndex - 4);
                                }
                            }
#pragma omp section
                            {
                                // Right
                                if (!IS_TO_DRAW(x + 1, y, z, CX, CZ) && !IS_TO_DRAW(x,y,z, CX+1, CZ))
                                {
                                    RightIndices.push_back(RightIndex++);
                                    PUSH_BACK_VERTEX(RightVertices,x + 0.5f + CHUNK(CX,CZ)->GetWorldX(), y - 0.5f, z + 0.5f + CHUNK(CX,CZ)->GetWorldZ(), 1.0f, 0.0f, 0.0f, u + SideShift, v);

                                    RightIndices.push_back(RightIndex++);
                                    PUSH_BACK_VERTEX(RightVertices,x + 0.5f + CHUNK(CX,CZ)->GetWorldX(), y - 0.5f, z - 0.5f + CHUNK(CX,CZ)->GetWorldZ(), 1.0f, 0.0f, 0.0f, u + SideShift+uvsize, v);

                                    RightIndices.push_back(RightIndex);
                                    RightIndices.push_back(RightIndex++);
                                    PUSH_BACK_VERTEX(RightVertices,x + 0.5f + CHUNK(CX,CZ)->GetWorldX(), y + 0.5f, z - 0.5f + CHUNK(CX,CZ)->GetWorldZ(), 1.0f, 0.0f, 0.0f, u + SideShift + uvsize, v + uvsize);

                                    RightIndices.push_back(RightIndex++);
                                    PUSH_BACK_VERTEX(RightVertices,x + 0.5f + CHUNK(CX,CZ)->GetWorldX(), y + 0.5f, z + 0.5f + CHUNK(CX,CZ)->GetWorldZ(), 1.0f, 0.0f, 0.0f, u + SideShift,v + uvsize);

                                    RightIndices.push_back(RightIndex - 4);
                                }
                            }
#pragma omp section
                            {
                                // Left
                                if (!IS_TO_DRAW(x - 1, y, z, CX, CZ) && !IS_TO_DRAW(x,y,z, CX-1, CZ))
                                {
                                    LeftIndices.push_back(LeftIndex++);
                                    PUSH_BACK_VERTEX(LeftVertices,x - 0.5f + CHUNK(CX,CZ)->GetWorldX(), y - 0.5f, z - 0.5f + CHUNK(CX,CZ)->GetWorldZ(), -1.0f, 0.0f, 0.0f, u + SideShift, v);

                                    LeftIndices.push_back(LeftIndex++);
                                    PUSH_BACK_VERTEX(LeftVertices,x - 0.5f + CHUNK(CX,CZ)->GetWorldX(), y - 0.5f, z + 0.5f + CHUNK(CX,CZ)->GetWorldZ(), -1.0f, 0.0f, 0.0f, u + SideShift+uvsize, v);

                                    LeftIndices.push_back(LeftIndex);
                                    LeftIndices.push_back(LeftIndex++);
                                    PUSH_BACK_VERTEX(LeftVertices,x - 0.5f + CHUNK(CX,CZ)->GetWorldX(), y + 0.5f, z + 0.5f + CHUNK(CX,CZ)->GetWorldZ(), -1.0f, 0.0f, 0.0f, u + SideShift + uvsize, v + uvsize);

                                    LeftIndices.push_back(LeftIndex++);
                                    PUSH_BACK_VERTEX(LeftVertices,x - 0.5f + CHUNK(CX,CZ)->GetWorldX(), y + 0.5f, z - 0.5f + CHUNK(CX,CZ)->GetWorldZ(), -1.0f, 0.0f, 0.0f, u + SideShift, v + uvsize);

                                    LeftIndices.push_back(LeftIndex - 4);
                                }
                            }
#pragma omp section
                            {
                                // Top
                                if (!IS_TO_DRAW(x, y + 1, z, CX, CZ))
                                {
                                    TopIndices.push_back(TopIndex++);
                                    PUSH_BACK_VERTEX(TopVertices,x - 0.5f + CHUNK(CX,CZ)->GetWorldX(), y + 0.5f, z + 0.5f + CHUNK(CX,CZ)->GetWorldZ(), 0.0f, 1.0f, 0.0f, u, v);

                                    TopIndices.push_back(TopIndex++);
                                    PUSH_BACK_VERTEX(TopVertices,x + 0.5f + CHUNK(CX,CZ)->GetWorldX(), y + 0.5f, z + 0.5f + CHUNK(CX,CZ)->GetWorldZ(), 0.0f, 1.0f, 0.0f, u+uvsize, v);

                                    TopIndices.push_back(TopIndex);
                                    TopIndices.push_back(TopIndex++);
                                    PUSH_BACK_VERTEX(TopVertices,x + 0.5f + CHUNK(CX,CZ)->GetWorldX(), y + 0.5f, z - 0.5f + CHUNK(CX,CZ)->GetWorldZ(), 0.0f, 1.0f, 0.0f, u + uvsize, v + uvsize);

                                    TopIndices.push_back(TopIndex++);
                                    PUSH_BACK_VERTEX(TopVertices,x - 0.5f + CHUNK(CX,CZ)->GetWorldX(), y + 0.5f, z - 0.5f + CHUNK(CX,CZ)->GetWorldZ(), 0.0f, 1.0f, 0.0f, u,v + uvsize);

                                    TopIndices.push_back(TopIndex - 4);
                                }
                            }
#pragma omp section
                            {
                                // Bottom
                                if (!IS_TO_DRAW(x, y - 1, z, CX, CZ))
                                {
                                    BottomIndices.push_back(BottomIndex++);
                                    PUSH_BACK_VERTEX(BottomVertices,x - 0.5f + CHUNK(CX,CZ)->GetWorldX(), y - 0.5f, z - 0.5f + CHUNK(CX,CZ)->GetWorldZ(), 0.0f, -1.0f, 0.0f, u + BottomShift, v);

                                    BottomIndices.push_back(BottomIndex++);
                                    PUSH_BACK_VERTEX(BottomVertices,x + 0.5f + CHUNK(CX,CZ)->GetWorldX(), y - 0.5f, z - 0.5f + CHUNK(CX,CZ)->GetWorldZ(), 0.0f, -1.0f, 0.0f, u + BottomShift+uvsize, v);

                                    BottomIndices.push_back(BottomIndex);
                                    BottomIndices.push_back(BottomIndex++);
                                    PUSH_BACK_VERTEX(BottomVertices,x + 0.5f + CHUNK(CX,CZ)->GetWorldX(), y - 0.5f, z + 0.5f + CHUNK(CX,CZ)->GetWorldZ(), 0.0f, -1.0f, 0.0f, u + BottomShift + uvsize, v + uvsize);

                                    BottomIndices.push_back(BottomIndex++);
                                    PUSH_BACK_VERTEX(BottomVertices,x - 0.5f + CHUNK(CX,CZ)->GetWorldX(), y - 0.5f, z + 0.5f + CHUNK(CX,CZ)->GetWorldZ(), 0.0f, -1.0f, 0.0f, u + BottomShift,v + uvsize);

                                    BottomIndices.push_back(BottomIndex - 4);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

#pragma omp parallel sections
    {
#pragma omp section
        {
            FrontVertices.shrink_to_fit();
            FrontIndices.shrink_to_fit();
        }
#pragma omp section
        {
            BackVertices.shrink_to_fit();
            BackIndices.shrink_to_fit();
        }
#pragma omp section
        {
            LeftVertices.shrink_to_fit();
            LeftIndices.shrink_to_fit();
        }
#pragma omp section
        {
            RightVertices.shrink_to_fit();
            RightIndices.shrink_to_fit();
        }
#pragma omp section
        {
            TopVertices.shrink_to_fit();
            TopIndices.shrink_to_fit();
        }
#pragma omp section
        {
            BottomVertices.shrink_to_fit();
            BottomIndices.shrink_to_fit();
        }
    }

    std::vector<Mesh*> ret;
    ret.push_back(new Mesh(FrontVertices, FrontIndices));
    ret.push_back(new Mesh(BackVertices, BackIndices));
    ret.push_back(new Mesh(RightVertices, RightIndices));
    ret.push_back(new Mesh(LeftVertices, LeftIndices));
    ret.push_back(new Mesh(TopVertices, TopIndices));
    ret.push_back(new Mesh(BottomVertices, BottomIndices));

    return ret;
}

World::~World()
{
    for(auto var : Chunks)
        delete var;
}

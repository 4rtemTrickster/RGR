#include "Help.h"
#include "../Mesh/Mesh.h"
#include "World.h"



/**
* \brief Check's for the presence of a voxel in the chunk
*/
#define IS_IN_CHUNK(X,Y,Z) (((X) >= 0 && (X) < Chunk::Chunk_Width) && ((Y) >= 0 && (Y) < Chunk::Chunk_Height) && ((Z) >= 0 && (Z) < Chunk::Chunk_Length))

/**
* \brief Check's for the presence of a Chunk in the World
*/
#define CHUNK_IN_WORLD(X,Z) (((X) >=0 && (X) < World_Width) && ((Z) >= 0 && (Z) < World_Length))

/**
* \brief Provides easier access to the Chunk
*/
#define CHUNK(X,Z) (this->Chunks[((Z) * World_Width) + (X)])

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
Vertices.push_back({glm::vec3(X,Y,Z), glm::vec3(NX,NY,NZ), glm::vec2(TX,TY)});

World::World()
{
    LOG_INFO("Start world generation");
    GLdouble start_time = glfwGetTime();
    
    Chunks.resize(World_Volume);


    for (int z = 0; z < World_Length; ++z)
    {
        const GLuint WorldZ = z * Chunk::Chunk_Length;
        for (int x = 0; x < World_Width; ++x)
            CHUNK(x, z) = new Chunk(x * Chunk::Chunk_Width, WorldZ);
        
    }

    
    LOG_INFO("World generated in {0} seconds", glfwGetTime() - start_time);
}

std::vector<Mesh*> World::GenerateMeshes()
{
    std::vector<Mesh*> ret;
    std::vector<std::pair<std::vector<Vertex>,std::vector<GLuint>>> VertexIndexData;
    VertexIndexData.resize(4);
    ret.reserve(4);
 
    
    auto lb = std::async(std::launch::async, [&]()
    {
       VertexIndexData[0] = ProcessBlock(0,0, World_Width/2, World_Length/2);
    });

    auto rb =std::async(std::launch::async, [&]()
    {
        VertexIndexData[1] = ProcessBlock(World_Width/2,0, World_Width, World_Length/2);
    });
    
    auto lt = std::async(std::launch::async, [&]()
    {
        VertexIndexData[2] =  ProcessBlock(0,World_Length/2, World_Width/2, World_Length);
    });
    
    auto rt =std::async(std::launch::async, [&]()
    {
        VertexIndexData[3] = ProcessBlock(World_Width/2, World_Length/2, World_Width, World_Length);
    });

    lb.wait();
    rb.wait();
    lt.wait();
    rt.wait();

    ret.push_back(new Mesh(VertexIndexData[0].first, VertexIndexData[0].second));
    ret.push_back(new Mesh(VertexIndexData[1].first, VertexIndexData[1].second));
    ret.push_back(new Mesh(VertexIndexData[2].first, VertexIndexData[2].second));
    ret.push_back(new Mesh(VertexIndexData[3].first, VertexIndexData[3].second));

    
    return ret;
}

std::pair<std::vector<Vertex>,std::vector<GLuint>> World::ProcessBlock(const GLuint XStart, const GLuint ZStart, const GLuint XBorder, const GLuint ZBorder)
{
    const GLfloat uvsize = 1.0f / 16.0f;
    const GLfloat SideShift = 1 * uvsize;
    const GLfloat BottomShift = 2 * uvsize;
    const GLuint AtlasShift = 3;

    std::vector<Vertex> Vertices;
    std::vector<GLuint> Indices;
    size_t Index = 0;

    auto v = std::async(std::launch::async, [&]()
    {
        Vertices.reserve(World_Volume / 4 * Chunk::Chunk_Volume * 4);
    });

    auto i = std::async(std::launch::async, [&]()
    {
        Indices.reserve(World_Volume / 4 * Chunk::Chunk_Volume * (4 + 6));
    });


    v.wait();
    i.wait();

    for (GLuint CZ = ZStart; CZ < ZBorder; ++CZ)
    {
        for (GLuint CX = XStart; CX < XBorder; ++CX)
        {
            for (GLuint y = 0; y < Chunk::Chunk_Height; ++y)
            {
                for (GLuint z = 0; z < Chunk::Chunk_Length; ++z)
                {
                    for (GLuint x = 0; x < Chunk::Chunk_Width; ++x)
                    {
                        const GLuint id = VOXEL(x, y, z, CX, CZ).id;

                        if (!id)
                            continue;

                        const float u = (id * AtlasShift % 16) * uvsize;
                        const float v = 1-((1 + id / 16) * uvsize);


                        // Front to def camera position
                        if (!IS_TO_DRAW(x, y, z + 1, CX, CZ) && (z == Chunk::Chunk_Length-1 ? !IS_TO_DRAW(x,y,0, CX, CZ + 1) : true))
                        {
                            Indices.push_back(Index++);
                            PUSH_BACK_VERTEX(x - 0.5f + CHUNK(CX,CZ)->GetWorldX(), y - 0.5f, z + 0.5f + CHUNK(CX,CZ)->GetWorldZ(), 0.0f, 0.0f, 1.0f, u + SideShift, v);

                            Indices.push_back(Index++);
                            PUSH_BACK_VERTEX(x + 0.5f + CHUNK(CX,CZ)->GetWorldX(), y - 0.5f, z + 0.5f + CHUNK(CX,CZ)->GetWorldZ(), 0.0f, 0.0f, 1.0f, u + SideShift+uvsize, v);

                            Indices.push_back(Index);
                            Indices.push_back(Index++);
                            PUSH_BACK_VERTEX(x + 0.5f + CHUNK(CX,CZ)->GetWorldX(), y + 0.5f, z + 0.5f + CHUNK(CX,CZ)->GetWorldZ(), 0.0f, 0.0f, 1.0f, u + SideShift + uvsize, v + uvsize);

                            Indices.push_back(Index++);
                            PUSH_BACK_VERTEX(x - 0.5f + CHUNK(CX,CZ)->GetWorldX(), y + 0.5f, z + 0.5f + CHUNK(CX,CZ)->GetWorldZ(), 0.0f, 0.0f, 1.0f, u + SideShift,v + uvsize);

                            Indices.push_back(Index - 4);
                        }

                        // Back
                        if (!IS_TO_DRAW(x, y, z - 1, CX, CZ) && (z == 0 ? !IS_TO_DRAW(x,y,Chunk::Chunk_Length-1, CX, CZ - 1) : true))
                        {
                            Indices.push_back(Index++);
                            PUSH_BACK_VERTEX(x + 0.5f + CHUNK(CX,CZ)->GetWorldX(), y - 0.5f, z - 0.5f + CHUNK(CX,CZ)->GetWorldZ(), 0.0f, 0.0f, -1.0f, u + SideShift, v);

                            Indices.push_back(Index++);
                            PUSH_BACK_VERTEX(x - 0.5f + CHUNK(CX,CZ)->GetWorldX(), y - 0.5f, z - 0.5f + CHUNK(CX,CZ)->GetWorldZ(), 0.0f, 0.0f, -1.0f, u + SideShift+uvsize, v);

                            Indices.push_back(Index);
                            Indices.push_back(Index++);
                            PUSH_BACK_VERTEX(x - 0.5f + CHUNK(CX,CZ)->GetWorldX(), y + 0.5f, z - 0.5f + CHUNK(CX,CZ)->GetWorldZ(), 0.0f, 0.0f, -1.0f, u + SideShift + uvsize, v + uvsize);

                            Indices.push_back(Index++);
                            PUSH_BACK_VERTEX(x + 0.5f + CHUNK(CX,CZ)->GetWorldX(), y + 0.5f, z - 0.5f + CHUNK(CX,CZ)->GetWorldZ(), 0.0f, 0.0f, -1.0f, u + SideShift,v + uvsize);

                            Indices.push_back(Index - 4);
                        }
 
                        // Right
                        if (!IS_TO_DRAW(x + 1, y, z, CX, CZ) && (x == Chunk::Chunk_Width-1 ? !IS_TO_DRAW(0,y,z, CX+1, CZ) : true))
                        {
                            Indices.push_back(Index++);
                            PUSH_BACK_VERTEX(x + 0.5f + CHUNK(CX,CZ)->GetWorldX(), y - 0.5f, z + 0.5f + CHUNK(CX,CZ)->GetWorldZ(), 1.0f, 0.0f, 0.0f, u + SideShift, v);

                            Indices.push_back(Index++);
                            PUSH_BACK_VERTEX(x + 0.5f + CHUNK(CX,CZ)->GetWorldX(), y - 0.5f, z - 0.5f + CHUNK(CX,CZ)->GetWorldZ(), 1.0f, 0.0f, 0.0f, u + SideShift+uvsize, v);

                            Indices.push_back(Index);
                            Indices.push_back(Index++);
                            PUSH_BACK_VERTEX(x + 0.5f + CHUNK(CX,CZ)->GetWorldX(), y + 0.5f, z - 0.5f + CHUNK(CX,CZ)->GetWorldZ(), 1.0f, 0.0f, 0.0f, u + SideShift + uvsize, v + uvsize);

                            Indices.push_back(Index++);
                            PUSH_BACK_VERTEX(x + 0.5f + CHUNK(CX,CZ)->GetWorldX(), y + 0.5f, z + 0.5f + CHUNK(CX,CZ)->GetWorldZ(), 1.0f, 0.0f, 0.0f, u + SideShift,v + uvsize);

                            Indices.push_back(Index - 4);
                        }
   
                        // Left
                        if (!IS_TO_DRAW(x - 1, y, z, CX, CZ) && (x == 0 ? !IS_TO_DRAW(Chunk::Chunk_Width-1,y,z, CX-1, CZ) : true))
                        {
                            Indices.push_back(Index++);
                            PUSH_BACK_VERTEX(x - 0.5f + CHUNK(CX,CZ)->GetWorldX(), y - 0.5f, z - 0.5f + CHUNK(CX,CZ)->GetWorldZ(), -1.0f, 0.0f, 0.0f, u + SideShift, v);

                            Indices.push_back(Index++);
                            PUSH_BACK_VERTEX(x - 0.5f + CHUNK(CX,CZ)->GetWorldX(), y - 0.5f, z + 0.5f + CHUNK(CX,CZ)->GetWorldZ(), -1.0f, 0.0f, 0.0f, u + SideShift+uvsize, v);

                            Indices.push_back(Index);
                            Indices.push_back(Index++);
                            PUSH_BACK_VERTEX(x - 0.5f + CHUNK(CX,CZ)->GetWorldX(), y + 0.5f, z + 0.5f + CHUNK(CX,CZ)->GetWorldZ(), -1.0f, 0.0f, 0.0f, u + SideShift + uvsize, v + uvsize);

                            Indices.push_back(Index++);
                            PUSH_BACK_VERTEX(x - 0.5f + CHUNK(CX,CZ)->GetWorldX(), y + 0.5f, z - 0.5f + CHUNK(CX,CZ)->GetWorldZ(), -1.0f, 0.0f, 0.0f, u + SideShift, v + uvsize);

                            Indices.push_back(Index - 4);
                        }

                        // Top
                        if (!IS_TO_DRAW(x, y + 1, z, CX, CZ))
                        {
                            Indices.push_back(Index++);
                            PUSH_BACK_VERTEX(x - 0.5f + CHUNK(CX,CZ)->GetWorldX(), y + 0.5f, z + 0.5f + CHUNK(CX,CZ)->GetWorldZ(), 0.0f, 1.0f, 0.0f, u, v);

                            Indices.push_back(Index++);
                            PUSH_BACK_VERTEX(x + 0.5f + CHUNK(CX,CZ)->GetWorldX(), y + 0.5f, z + 0.5f + CHUNK(CX,CZ)->GetWorldZ(), 0.0f, 1.0f, 0.0f, u+uvsize, v);

                            Indices.push_back(Index);
                            Indices.push_back(Index++);
                            PUSH_BACK_VERTEX(x + 0.5f + CHUNK(CX,CZ)->GetWorldX(), y + 0.5f, z - 0.5f + CHUNK(CX,CZ)->GetWorldZ(), 0.0f, 1.0f, 0.0f, u + uvsize, v + uvsize);

                            Indices.push_back(Index++);
                            PUSH_BACK_VERTEX(x - 0.5f + CHUNK(CX,CZ)->GetWorldX(), y + 0.5f, z - 0.5f + CHUNK(CX,CZ)->GetWorldZ(), 0.0f, 1.0f, 0.0f, u,v + uvsize);

                            Indices.push_back(Index - 4);
                        }
                        
                        // Bottom
                        if (!IS_TO_DRAW(x, y - 1, z, CX, CZ))
                        {
                            Indices.push_back(Index++);
                            PUSH_BACK_VERTEX(x - 0.5f + CHUNK(CX,CZ)->GetWorldX(), y - 0.5f, z - 0.5f + CHUNK(CX,CZ)->GetWorldZ(), 0.0f, -1.0f, 0.0f, u + BottomShift, v);

                            Indices.push_back(Index++);
                            PUSH_BACK_VERTEX(x + 0.5f + CHUNK(CX,CZ)->GetWorldX(), y - 0.5f, z - 0.5f + CHUNK(CX,CZ)->GetWorldZ(), 0.0f, -1.0f, 0.0f, u + BottomShift+uvsize, v);

                            Indices.push_back(Index);
                            Indices.push_back(Index++);
                            PUSH_BACK_VERTEX(x + 0.5f + CHUNK(CX,CZ)->GetWorldX(), y - 0.5f, z + 0.5f + CHUNK(CX,CZ)->GetWorldZ(), 0.0f, -1.0f, 0.0f, u + BottomShift + uvsize, v + uvsize);

                            Indices.push_back(Index++);
                            PUSH_BACK_VERTEX(x - 0.5f + CHUNK(CX,CZ)->GetWorldX(), y - 0.5f, z + 0.5f + CHUNK(CX,CZ)->GetWorldZ(), 0.0f, -1.0f, 0.0f, u + BottomShift,v + uvsize);

                            Indices.push_back(Index - 4);
                        }
                    }
                }
            }
        }
    }

    Vertices.shrink_to_fit();
    Indices.shrink_to_fit();

    
    
    return std::make_pair(Vertices, Indices);
}

World::~World()
{
    for(auto var : Chunks)
        delete var;
}

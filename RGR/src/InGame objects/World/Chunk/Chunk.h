#pragma once

#include "Help.h"
#include "Voxel/Voxel.h"

class Chunk
{
public:
    //Chunk params
    static const GLuint Chunk_Width = 16;
    static const GLuint Chunk_Length = 16;
    static const GLuint Chunk_Height = 128;
    static const GLuint Chunk_Volume = Chunk_Width * Chunk_Length * Chunk_Height;

    Chunk(GLint WorldX, GLint WorldZ);

    ~Chunk();

    std::vector<Voxel>& GetVoxels() { return voxels;}

    GLint GetWorldX() const { return WorldX;}
    GLint GetWorldZ() const { return WorldZ;}


protected:
    std::vector<Voxel> voxels;

    GLint WorldX;
    GLint WorldZ;
};

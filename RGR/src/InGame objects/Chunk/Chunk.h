#pragma once

#include "../../Help.h"
#include "../Mesh/Mesh.h"
#include "Voxel/Voxel.h"

class Chunk
{
private:

	Voxel* voxels;

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

public:
	//Chunk params
	static const GLuint Chunk_Width = 16;
	static const GLuint Chunk_Length = 16;
	static const GLuint Chunk_Height = 16;
	static const GLuint Chunk_Volume = Chunk_Width * Chunk_Length * Chunk_Height;

	Chunk();
	~Chunk();

	Mesh* GenerateMesh();
};

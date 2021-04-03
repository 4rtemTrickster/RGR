#pragma once

#include "../../../Help.h"
#include "../../Mesh/Mesh.h"
#include "Voxel/Voxel.h"

class Chunk
{
public:
	Chunk(const Chunk& other)
		: WorldX(other.WorldX),
		  WorldZ(other.WorldZ),
		  vertices(other.vertices),
		  indices(other.indices)
	{
	}

	Chunk(Chunk&& other) noexcept
		: WorldX(other.WorldX),
		  WorldZ(other.WorldZ),
		  vertices(std::move(other.vertices)),
		  indices(std::move(other.indices))
	{
	}

	Chunk& operator=(const Chunk& other)
	{
		if (this == &other)
			return *this;
		WorldX = other.WorldX;
		WorldZ = other.WorldZ;
		vertices = other.vertices;
		indices = other.indices;
		return *this;
	}

	Chunk& operator=(Chunk&& other) noexcept
	{
		if (this == &other)
			return *this;
		WorldX = other.WorldX;
		WorldZ = other.WorldZ;
		vertices = std::move(other.vertices);
		indices = std::move(other.indices);
		return *this;
	}

	//Chunk params
	static const GLuint Chunk_Width = 16;
	static const GLuint Chunk_Length = 16;
	static const GLuint Chunk_Height = 128;
	static const GLuint Chunk_Volume = Chunk_Width * Chunk_Length * Chunk_Height;

	Chunk(GLint WorldX, GLint WorldZ);
	Chunk() = default;

	~Chunk();

	void Init(GLint WorldX, GLint WorldZ);

	Mesh* GenerateMesh();

private:

	Voxel* voxels = nullptr;
	GLint WorldX;
	GLint WorldZ;

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	bool initializeflag = false;
};

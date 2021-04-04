#pragma once

#include "../../../Help.h"
#include "../../Mesh/Mesh.h"
#include "Voxel/Voxel.h"

class Chunk
{
public:
	//Chunk params
	static const GLuint Chunk_Width = 16;
	static const GLuint Chunk_Length = 16;
	static const GLuint Chunk_Height = 128;
	static const GLuint Chunk_Volume = Chunk_Width * Chunk_Length * Chunk_Height;

	Chunk(const Chunk& other);
	Chunk(Chunk&& other) noexcept;
	Chunk& operator=(const Chunk& other);
	Chunk& operator=(Chunk&& other) noexcept;
	Chunk(GLint WorldX, GLint WorldZ);
	Chunk() = default;

	~Chunk();

	inline bool IsInit() const { return bIsInit;}

	void Init(GLint WorldX, GLint WorldZ);

	Mesh* GenerateMesh();

private:

	Voxel* voxels = nullptr;
	GLint WorldX;
	GLint WorldZ;

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	bool bIsInit = false;
};

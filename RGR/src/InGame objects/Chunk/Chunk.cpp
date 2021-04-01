#include "Chunk.h"

#include "../Mesh/Mesh.h"
#include "glm/glm.hpp"

/**
 * \brief Ñhecks for the presence of a voxel in the chunk
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


Chunk::Chunk()
{
	this->voxels = new Voxel[Chunk_Volume];
	
	for (size_t y = 0; y < Chunk_Height; y++)
	{
		for (size_t z = 0; z < Chunk_Length; z++)
		{
			for (size_t x = 0; x < Chunk_Width; x++)
			{
				VOXEL(x, y, z).id = y <= (std::sin(x * 0.6f) * 0.5f + 0.5f) * 10;
			}
		}
	}
}

Chunk::~Chunk()
{
	
}

Mesh* Chunk::GenerateMesh()
{	
	size_t index = 0;

	vertices.reserve(Chunk_Volume*6);
	indices.reserve(Chunk_Volume*6);

	for (size_t y = 0; y < Chunk_Height; y++)
	{
		for (size_t z = 0; z < Chunk_Length; z++)
		{
			for (size_t x = 0; x < Chunk_Width; x++)
			{
				if (!(VOXEL(x, y, z).id))
					continue;

				// Front to def camera position
				if (!IS_TO_DRAW(x, y, z + 1))
				{
					indices.push_back(index++);
					PUSH_BACK_VERTEX(x - 0.5f, y - 0.5f, z + 0.5f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f);

					indices.push_back(index++);
					PUSH_BACK_VERTEX(x + 0.5f, y - 0.5f, z + 0.5f,	0.0f, 0.0f, 1.0f,	1.0f, 0.0f);

					indices.push_back(index);
					indices.push_back(index++);
					PUSH_BACK_VERTEX(x + 0.5f, y + 0.5f, z + 0.5f,	0.0f, 0.0f, 1.0f,	1.0f, 1.0f);

					indices.push_back(index++);
					PUSH_BACK_VERTEX(x - 0.5f, y + 0.5f, z + 0.5f,	0.0f, 0.0f, 1.0f,	0.0f, 1.0f);

					indices.push_back(index - 4);
				}

				// Back
				if (!IS_TO_DRAW(x, y, z - 1))
				{
					indices.push_back(index++);
					PUSH_BACK_VERTEX(x + 0.5f, y - 0.5f, z - 0.5f,	0.0f, 0.0f, -1.0f,	0.0f, 0.0f);

					indices.push_back(index++);
					PUSH_BACK_VERTEX(x - 0.5f, y - 0.5f, z - 0.5f,	0.0f, 0.0f, -1.0f,	1.0f, 0.0f);

					indices.push_back(index);
					indices.push_back(index++);
					PUSH_BACK_VERTEX(x - 0.5f, y + 0.5f, z - 0.5f,	0.0f, 0.0f, -1.0f,	1.0f, 1.0f);

					indices.push_back(index++);
					PUSH_BACK_VERTEX(x + 0.5f, y + 0.5f, z - 0.5f,	0.0f, 0.0f, -1.0f,	0.0f, 1.0f);

					indices.push_back(index - 4);
				}

				// Right
				if (!IS_TO_DRAW(x + 1, y, z))
				{
					indices.push_back(index++);
					PUSH_BACK_VERTEX(x + 0.5f, y - 0.5f, z + 0.5f,	1.0f, 0.0f, 0.0f,	0.0f, 0.0f);

					indices.push_back(index++);
					PUSH_BACK_VERTEX(x + 0.5f, y - 0.5f, z - 0.5f,	1.0f, 0.0f, 0.0f,	1.0f, 0.0f);

					indices.push_back(index);
					indices.push_back(index++);
					PUSH_BACK_VERTEX(x + 0.5f, y + 0.5f, z - 0.5f,	1.0f, 0.0f, 0.0f,	1.0f, 1.0f);

					indices.push_back(index++);
					PUSH_BACK_VERTEX(x + 0.5f, y + 0.5f, z + 0.5f,	1.0f, 0.0f, 0.0f,	0.0f, 1.0f);

					indices.push_back(index - 4);
				}

				// Left
				if (!IS_TO_DRAW(x - 1, y, z))
				{
					indices.push_back(index++);
					PUSH_BACK_VERTEX(x - 0.5f, y - 0.5f, z - 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);

					indices.push_back(index++);
					PUSH_BACK_VERTEX(x - 0.5f, y - 0.5f, z + 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

					indices.push_back(index);
					indices.push_back(index++);
					PUSH_BACK_VERTEX(x - 0.5f, y + 0.5f, z + 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

					indices.push_back(index++);
					PUSH_BACK_VERTEX(x - 0.5f, y + 0.5f, z - 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);

					indices.push_back(index - 4);
				}

				// Top
				if (!IS_TO_DRAW(x, y + 1, z))
				{
					indices.push_back(index++);
					PUSH_BACK_VERTEX(x - 0.5f, y + 0.5f, z + 0.5f,	0.0f, 1.0f, 0.0f,	0.0f, 0.0f);

					indices.push_back(index++);
					PUSH_BACK_VERTEX(x + 0.5f, y + 0.5f, z + 0.5f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f);

					indices.push_back(index);
					indices.push_back(index++);
					PUSH_BACK_VERTEX(x + 0.5f, y + 0.5f, z - 0.5f,	0.0f, 1.0f, 0.0f,	1.0f, 1.0f);

					indices.push_back(index++);
					PUSH_BACK_VERTEX(x - 0.5f, y + 0.5f, z - 0.5f,	0.0f, 1.0f, 0.0f,	0.0f, 1.0f);

					indices.push_back(index - 4);
				}

				// Bottom
				if (!IS_TO_DRAW(x, y - 1, z))
				{
					indices.push_back(index++);
					PUSH_BACK_VERTEX(x - 0.5f, y - 0.5f, z - 0.5f,	0.0f, -1.0f, 0.0f,	0.0f, 0.0f);

					indices.push_back(index++);
					PUSH_BACK_VERTEX(x + 0.5f, y - 0.5f, z - 0.5f,	0.0f, -1.0f, 0.0f,	1.0f, 0.0f);

					indices.push_back(index);
					indices.push_back(index++);
					PUSH_BACK_VERTEX(x + 0.5f, y - 0.5f, z + 0.5f,	0.0f, -1.0f, 0.0f,	1.0f, 1.0f);

					indices.push_back(index++);
					PUSH_BACK_VERTEX(x - 0.5f, y - 0.5f, z + 0.5f,	0.0f, -1.0f, 0.0f,	0.0f, 1.0f);

					indices.push_back(index - 4);
				}
			}
		}
	}

	vertices.shrink_to_fit();
	indices.shrink_to_fit();

	return new Mesh(vertices, indices);
}

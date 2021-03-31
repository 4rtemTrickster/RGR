#include "Chunk.h"

Chunk::Chunk()
	: Texture("res/Textures/Box Texture/Diffuse.png")
{

	for (size_t i = 0; i < Chunk_Height; i++)
	{
		for (size_t j = 0; j < Chunk_Length; j++)
		{
			for (size_t k = 0; k < Chunk_Width; k++)
			{
				m_Positions.push_back(static_cast<GLfloat>(k));
			}
		}
	}
}

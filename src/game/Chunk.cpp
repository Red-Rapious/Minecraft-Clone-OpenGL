#include "Chunk.hpp"

Chunk::Chunk(ChunkCoord coord)
	: m_coord(coord), m_blocksArray()
{
	for (unsigned int x = 0; x < CHUNK_X_BLOCK_COUNT; x++)
	{
		for (unsigned int y = 0; y <CHUNK_Y_BLOCK_COUNT ; y++)
			m_blocksArray.reserve(CHUNK_Z_BLOCK_COUNT);
	}
}

inline void Chunk::SetBlockType(glm::vec3 block_position, BlockType type)
{
	m_blocksArray[(unsigned int)block_position.x][(unsigned int)block_position.y][(unsigned int)block_position.z] = type;
}

void Chunk::FillPlaneWithBlocks(unsigned int height, BlockType type)
{
	for (unsigned int i = 0; i < 15 ; i++)
	{
		for (unsigned int j = 0 ; j < 15; j++)
		{
			m_blocksArray[i][height][j] = type;
		}
	}
}

float* Chunk::GetVertexBufferToRender() const
{
	return nullptr;
}

void Chunk::UpdateVertexBufferToRender()
{
	for (unsigned int x = 0; x < 16; x++)
	{

	}
}

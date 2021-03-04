#include "Chunk.hpp"

Chunk::Chunk(ChunkCoord coord)
	: m_coord(coord), m_blocksArray()
{
}

inline void Chunk::SetBlockType(glm::vec3 block_position, BlockType type)
{
	m_blocksArray[(int)block_position.x][(int)block_position.y][(int)block_position.z] = type;
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

#include "Chunck.h"

Chunck::Chunck(ChunckCoord coord)
	: m_coord(coord), m_blocksArray()
{
}

inline void Chunck::SetBlockType(glm::vec3 block_position, BlockType type)
{
	m_blocksArray[(int)block_position.x][(int)block_position.y][(int)block_position.z] = type;
}

void Chunck::FillPlaneWithBlocks(unsigned int height, BlockType type)
{
	for (unsigned int i = 0; i < 15 ; i++)
	{
		for (unsigned int j = 0 ; j < 15; j++)
		{
			m_blocksArray[i][height][j] = type;
		}
	}
}

ChunckCoord::ChunckCoord(glm::vec2 coord)
	: m_x(coord.x), m_y(coord.y)
{
}

ChunckCoord::ChunckCoord(int x, int y)
	:m_x(x), m_y(y)
{
}

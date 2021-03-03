#include "Chunck.h"

Chunck::Chunck(ChunckCoord coord)
	: m_coord(coord), m_blocksArray()
{
}

inline void Chunck::SetBlockType(glm::vec3 block_position, BlockType type)
{
	m_blocksArray[(int)block_position.x][(int)block_position.y][(int)block_position.z] = type;
}

ChunckCoord::ChunckCoord(glm::vec2 coord)
	: m_x(coord.x), m_y(coord.y)
{
}

ChunckCoord::ChunckCoord(int x, int y)
	:m_x(x), m_y(y)
{
}

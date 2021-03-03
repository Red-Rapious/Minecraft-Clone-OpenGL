#include "Chunck.h"

Chunck::Chunck(glm::vec2 position)
	: m_position(position), m_blocksArray()
{

}

inline void Chunck::SetBlockType(glm::vec3 block_position, BlockType type)
{
	m_blocksArray[(int)block_position.x][(int)block_position.y][(int)block_position.z] = type;
}

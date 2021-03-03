#pragma once

#include "glm/glm.hpp"

enum class BlockType
{
	GRASS
};

class Chunck
{
private:
	BlockType m_blocksArray[16][255][16];
	glm::vec2 m_position;

public:
	Chunck(glm::vec2 position);
	inline void SetBlockType(glm::vec3 block_position, BlockType type);
};
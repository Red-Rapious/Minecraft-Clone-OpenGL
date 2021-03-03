#pragma once

#include "glm/glm.hpp"

enum class BlockType
{
	GRASS
};

struct ChunckCoord
{
	int m_x;
	int m_y;

	ChunckCoord(glm::vec2 coord);
	ChunckCoord(int x=0, int y=0);

	inline bool operator==(const ChunckCoord& coord) const { return coord.m_x == m_x && coord.m_y == m_y; };
};

class ChunkCoordHash
{
public:
	size_t operator()(const ChunckCoord& coord) const
	{
		return coord.m_x + coord.m_y;
	}
};

class Chunck
{
private:
	BlockType m_blocksArray[16][255][16];
	ChunckCoord m_coord;

public:
	Chunck(ChunckCoord coord);
	inline void SetBlockType(glm::vec3 block_position, BlockType type);
	inline ChunckCoord GetCoord() const { return m_coord; };
};


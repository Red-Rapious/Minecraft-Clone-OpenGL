#pragma once

#include "glm/glm.hpp"

enum class BlockType
{
	NONE, GRASS
};


/* Code created by @PolarToCartesian */
struct ChunkCoord {
    std::int16_t idx;
    std::int16_t idz;

    inline bool operator==(const ChunkCoord& rhs) const noexcept { return this->idx == rhs.idx && this->idz == rhs.idz; }
};

class ChunkCoordHash {
public:
    inline size_t operator()(const ChunkCoord& cc) const
    {
        const std::uint16_t a = ((cc.idx < 0 ? (static_cast<std::uint16_t>(1u) << 15) : 0) | static_cast<std::uint16_t>(std::abs(cc.idx)));
        const std::uint16_t b = ((cc.idz < 0 ? (static_cast<std::uint16_t>(1u) << 15) : 0) | static_cast<std::uint16_t>(std::abs(cc.idz)));

        return (static_cast<std::uint32_t>(a) << 16u) | static_cast<std::uint32_t>(b);
    }
};



class Chunk
{
private:
	BlockType m_blocksArray[16][255][16];
	ChunkCoord m_coord;

public:
	Chunk(ChunkCoord coord);
	void SetBlockType(glm::vec3 block_position, BlockType type);
	void FillPlaneWithBlocks(unsigned int height, BlockType type);

	inline ChunkCoord GetCoord() const { return m_coord; };

};


#pragma once

#include "glm/glm.hpp"
#include <vector>
#include "glm/glm.hpp"
#include "Constants.hpp"
#include "../graphics/VertexIndexBufferCouple.hpp"

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
/* */

enum class FaceType
{
    FRONT, BACK, RIGHT, LEFT, UP, BELLOW
};

class Chunk
{
private:
	std::vector<std::vector<std::vector<BlockType>>> m_blocksArray;
	ChunkCoord m_coord;
    VertexIndexBufferCouple m_vertexIndexBufferCouple;
    unsigned int m_indexCount;
    
    void AddVertexToVertexBuffer(glm::vec3 vertexCoord, glm::vec2 textureCoord);
    void ClearVertexBuffer();
    void AddFaceToCouple(FaceType face_type, glm::vec3 block_coord, BlockType blockType);
    void UpdateInsideCoupleToRender();
    void UpdateOutsideCoupleToRender(const bool& chunkNorth, const bool& chunkSouth, const bool& chunkWest, const bool& chunkEast);

public:
	Chunk(ChunkCoord coord);
	void SetBlockType(glm::vec3 blockPosition, BlockType type);
    inline BlockType GetBlockType(const glm::vec3& blockPosition) const { return m_blocksArray[blockPosition.x][blockPosition.y][blockPosition.z]; };
	void FillPlaneWithBlocks(unsigned int height, BlockType type);
    void DeleteAllBlocks();

	inline ChunkCoord GetCoord() const { return m_coord; };
    VertexIndexBufferCouple GetCoupleToRender(const unsigned int& originIndex = 0, const bool& chunkChanges=false, const bool& chunkNorth = false, const bool& chunkSouth = false, const bool& chunkWest = false, const bool& chunkEast = false);

    unsigned int GetNumberOfNonAirBlocks(const bool& out=false) const;
};


#pragma once

#include "glm/glm.hpp"
#include <vector>
#include "glm/glm.hpp"
#include "Constants.hpp"
#include "../graphics/VertexIndexBufferCouple.hpp"
#include <unordered_map>
#include <memory>

enum class BlockType
{
	NONE, GRASS, DIRT, ROCK, SAND, LEAFS, WOOD
};


/* Code created by @PolarToCartesian */
struct ChunkCoord {
    std::int16_t idx;
    std::int16_t idz;

    inline ChunkCoord(int16_t x, int16_t z) : idx(x), idz(z) {};
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
    FRONT, BACK, RIGHT, LEFT, UP, DOWN
};

class Chunk
{
private:
	std::vector<std::vector<std::vector<BlockType>>> m_blocksArray;
	ChunkCoord m_coord;
    VertexIndexBufferCouple m_vertexIndexBufferCouple;
    
    void AddVertexToVertexBuffer(glm::vec3 vertexCoord, glm::vec2 textureCoord);
    void ClearVertexBuffer();
    void AddFaceToCouple(FaceType face_type, glm::vec3 block_coord, BlockType blockType);
   
public:
	Chunk(ChunkCoord coord);
    inline ChunkCoord GetCoord() const { return m_coord; };

    // Utility functions
    void SetBlockType(glm::vec3 blockPosition, BlockType type);
    void DeleteAllBlocks();
    inline BlockType GetBlockType(const glm::vec3& blockPosition) const { return m_blocksArray[blockPosition.x][blockPosition.y][blockPosition.z]; };
	void FillPlaneWithBlocks(unsigned int height, BlockType type);
    
    unsigned int GetNumberOfNonAirBlocks(const bool& out = false) const;

    // Render functions
    VertexIndexBufferCouple GetCoupleToRender(const unsigned int& originIndex, const std::unordered_map<ChunkCoord, std::unique_ptr<Chunk>, ChunkCoordHash>& chunksUMap);
    void RenderAllFacesNeeded(const std::unordered_map<ChunkCoord, std::unique_ptr<Chunk>, ChunkCoordHash>& chunksUMap);
};
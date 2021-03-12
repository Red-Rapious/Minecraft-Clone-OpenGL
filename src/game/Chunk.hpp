#pragma once

#include "glm/glm.hpp"
#include <vector>
#include "glm/glm.hpp"
#include "Constants.hpp"
#include "../graphics/VertexIndexBufferCouple.hpp"
#include <unordered_map>
#include <memory>

#include "../graphics/VertexBuffer.h"
#include "../graphics/IndexBuffer.h"
#include "../graphics/Renderer.h"
#include "../graphics/VertexArray.h"

enum class BlockType
{
	NONE, GRASS, DIRT, ROCK, SAND, LEAFS, LOG, PLANKS, BEDROCK, COAL, IRON
};

enum class FaceType
{
    FRONT, BACK, RIGHT, LEFT, UP, DOWN
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


class Chunk
{
private:
	std::vector<std::vector<std::vector<BlockType>>> m_blocksArray;
	ChunkCoord m_coord;

    // Rendering members
    VertexIndexBufferCouple m_vertexIndexBufferCouple;
    unsigned int m_vertexBuffer;
    unsigned int m_indexBuffer;
    
    void AddVertexToVertexBuffer(const glm::vec3& vertexCoord, const glm::vec2& textureCoord);
    void ClearVertexBuffer();
    void AddFaceToCouple(const FaceType& faceType, const glm::vec3& blockCoord, const BlockType& blockType);
   
public:
	Chunk(const ChunkCoord& coord);
    inline ChunkCoord GetCoord() const { return m_coord; };

    // Utility functions
    void SetBlockType(const glm::vec3& blockPosition, const BlockType& type);
    void DeleteAllBlocks();
    inline BlockType GetBlockType(const glm::vec3& blockPosition) const { return m_blocksArray[blockPosition.x][blockPosition.y][blockPosition.z]; };
	void FillPlaneWithBlocks(const unsigned int& height, const BlockType& type);
    
    // Debug functions
    unsigned int GetNumberOfNonAirBlocks(const bool& out = false) const;

    void Generate();

    // Render functions
    void RenderChunk(const VertexArray& vao, const std::unordered_map<ChunkCoord, std::unique_ptr<Chunk>, ChunkCoordHash>& chunksUMap);
    VertexIndexBufferCouple GetCoupleToRender(const std::unordered_map<ChunkCoord, std::unique_ptr<Chunk>, ChunkCoordHash>& chunksUMap);
    void ListAllFacesToRender(const std::unordered_map<ChunkCoord, std::unique_ptr<Chunk>, ChunkCoordHash>& chunksUMap);
};
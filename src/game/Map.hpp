#pragma once
#include <unordered_map>
#include "../graphics/VertexIndexBufferCouple.hpp"
#include "glm/glm.hpp"
#include <memory>

class Map
{
private:
	std::unordered_map<ChunkCoord, std::unique_ptr<Chunk>, ChunkCoordHash> m_chunksUMap;
	std::vector<Chunk> m_chunkVector;
	VertexIndexBufferCouple m_worldCouple;

	ChunkCoord ConvertPositionToChunkCoord(const glm::vec3& position);

public:
	void AddChunkToMap(const Chunk& chunk);
	VertexIndexBufferCouple GetCoupleToRender(ChunkCoord chunkPlayerPosition);
	inline Chunk* GetChunkByCoord(const ChunkCoord& coord) const { return m_chunksUMap.at(coord).get(); };
	void UpdateChunkGeneration(const glm::vec3& cameraPosition);
};
#pragma once
#include <unordered_map>
#include "../graphics/VertexIndexBufferCouple.hpp"
#include "glm/glm.hpp"
#include <memory>

class Map
{
private:
	std::unordered_map<ChunkCoord, std::unique_ptr<Chunk>, ChunkCoordHash> m_chunksUMap;
	VertexIndexBufferCouple m_worldCouple;
	ChunkCoord m_playerPosition;

	ChunkCoord ConvertPositionToChunkCoord(const glm::vec3& position);
	std::vector<ChunkCoord> GetChunksCoordsToRender();

public:
	inline Map() : m_playerPosition(ChunkCoord(0, 0)) {};
	void AddChunkToMap(const Chunk& chunk);
	VertexIndexBufferCouple GetCoupleToRender(ChunkCoord chunkPlayerPosition);
	inline Chunk* GetChunkByCoord(const ChunkCoord& coord) const { return m_chunksUMap.at(coord).get(); };
	void UpdateChunkGeneration();
	void UpdatePlayerPosition(const glm::vec3& cameraPosition);
};
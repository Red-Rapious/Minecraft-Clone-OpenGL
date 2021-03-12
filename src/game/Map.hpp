#pragma once
#include <unordered_map>
#include "../graphics/VertexIndexBufferCouple.hpp"
#include "glm/glm.hpp"
#include <memory>

class Map
{
private:
	std::unordered_map<ChunkCoord, std::unique_ptr<Chunk>, ChunkCoordHash> m_chunksUMap;
	ChunkCoord m_playerPosition;
	std::vector<ChunkCoord> m_chunkGenerationQueue;
	void AddChunkToGenQueue(const ChunkCoord& chunkCoord);
	unsigned int calculateChunksDistance(const ChunkCoord& coord1, const ChunkCoord& coord2=ChunkCoord(0,0));

	ChunkCoord ConvertPositionToChunkCoord(const glm::vec3& position);
	void RenderChunk(const ChunkCoord& coord, std::vector<ChunkCoord>& chunksCoordToRender);
	

public:
	

	inline Map() : m_playerPosition(ChunkCoord(0, 0)), m_chunkGenerationQueue({}) {};
	void AddChunkToMap(const Chunk& chunk);
	void RenderAllNeededChunks(VertexArray& vao);
	inline Chunk* GetChunkByCoord(const ChunkCoord& coord) const { return m_chunksUMap.at(coord).get(); };
	bool GenerateOneChunk();
	void UpdatePlayerPosition(const glm::vec3& cameraPosition);

	std::vector<ChunkCoord> GetChunksCoordsToRender();
};
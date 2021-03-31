#pragma once
#include <unordered_map>
#include "../graphics/VertexIndexBufferCouple.hpp"
#include "glm/glm.hpp"
#include <memory>
#include <iostream>
#include "Chunk.hpp"

#include "../vendor/PerlinNoise.hpp"

class Map
{
private:
	// Members
	std::unordered_map<ChunkCoord, std::unique_ptr<Chunk>, ChunkCoordHash> m_chunksUMap;
	ChunkCoord m_playerPosition;
	std::vector<ChunkCoord> m_chunkGenerationQueue;
	std::vector<ChunkCoord> m_lastChunksToRender;
	siv::PerlinNoise m_noise;
	int m_seed;

	// Methods
	void AddChunkToGenQueue(const ChunkCoord& chunkCoord);
	void RenderSpecificChunk(const ChunkCoord& coord, std::vector<ChunkCoord>& chunksCoordToRender);
	std::vector<ChunkCoord> GetChunksCoordsToRender();

	// Utility methods
	ChunkCoord ConvertPositionToChunkCoord(const glm::vec3& position);
	unsigned int calculateChunksDistance(const ChunkCoord& coord1, const ChunkCoord& coord2 = ChunkCoord(0, 0));

public:
	Map();

	void AddChunkToMap(const Chunk& chunk);
	inline Chunk* GetChunkByCoord(const ChunkCoord& coord) const { return m_chunksUMap.at(coord).get(); };

	bool GenerateOneChunk();
	void RenderAllNeededChunks(const VertexArray& vao);
	void UpdateChunkPlayerPosition(const glm::vec3& cameraPosition);

	inline ChunkCoord GetPlayerPosition(bool out=false) const {
		if (out)
			std::cout << "Player position: x=" << m_playerPosition.idx << "  z=" << m_playerPosition.idz << "\n";
		return m_playerPosition;
	}
};
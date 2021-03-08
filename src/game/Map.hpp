#pragma once
#include <unordered_map>
#include "../graphics/VertexIndexBufferCouple.hpp"

class Map
{
private:
	std::unordered_map<ChunkCoord, Chunk*, ChunkCoordHash> m_chunksUMap;
	std::vector<Chunk> m_chunkVector;
	VertexIndexBufferCouple m_worldCouple;

public:
	void AddChunkToMap(Chunk chunk);
	VertexIndexBufferCouple GetCoupleToRender(ChunkCoord chunkPlayerPosition);
	inline Chunk* GetChunkByCoord(const ChunkCoord& coord) const { return m_chunksUMap.at(coord); };
};
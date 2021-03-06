#pragma once
#include <unordered_map>

class Map
{
private:
	std::unordered_map<ChunkCoord, Chunk*, ChunkCoordHash> m_chunksUMap;
	std::vector<Chunk> m_chunkVector;
	std::vector<float> m_worldVertexBuffer;

public:
	void AddChunkToMap(Chunk chunk);
	std::vector<float> GetVertexBufferToRender(ChunkCoord chunkPlayerPosition);
	inline Chunk* GetChunkByCoord(const ChunkCoord& coord) const { return m_chunksUMap.at(coord);  };
};
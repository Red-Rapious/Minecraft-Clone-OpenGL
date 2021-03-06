#pragma once
#include <unordered_map>

class Map
{
private:
	std::unordered_map<ChunkCoord, Chunk*, ChunkCoordHash> m_chunksUMap;
	std::vector<Chunk> m_chunkVector;
	unsigned int m_vertexCount;
	std::vector<float> m_worldVertexBuffer;

public:
	Map();
	void AddChunkToMap(Chunk chunk);
	const float* GetVertexBufferToRender(ChunkCoord chunkPlayerPosition);
	inline unsigned int GetVertexCount() const { return m_vertexCount; };
	inline Chunk* GetChunkByCoord(const ChunkCoord& coord) const { return m_chunksUMap.at(coord);  };
};
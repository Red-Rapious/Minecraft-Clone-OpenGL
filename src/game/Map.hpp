#pragma once
#include <unordered_map>

class Map
{
private:
	std::unordered_map<ChunkCoord, std::unique_ptr<Chunk>, ChunkCoordHash> m_chunksUMap;
	unsigned int m_vertexCount;
	std::vector<float> m_worldVertexBuffer;

public:
	void AddChunkToMap(Chunk chunk);
	const float* GetVertexBufferToRender(ChunkCoord chunkPlayerPosition);
	inline unsigned int GetVertexCount() const { return m_vertexCount; };
};
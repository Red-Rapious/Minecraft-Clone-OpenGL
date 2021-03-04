#pragma once
#include <unordered_map>

class Map
{
private:
	std::unordered_map<ChunkCoord, Chunk*, ChunkCoordHash> m_chunksUMap;
public:
	void AddChunkToMap(Chunk chunk);
	float* GetVertexBufferToRender();
};
#include "Chunk.hpp"
#include "Map.hpp"
#include <vector>

void Map::AddChunkToMap(Chunk chunk)
{
	m_chunksUMap[chunk.GetCoord()] = &chunk;
}

float* Map::GetVertexBufferToRender()
{
	std::vector<float> vertexBuffer;
	return nullptr;
}
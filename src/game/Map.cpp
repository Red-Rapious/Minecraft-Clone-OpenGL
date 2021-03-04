#include "Chunk.hpp"
#include "Map.hpp"
#include "glm/glm.hpp"
#include <vector>

void Map::AddChunkToMap(Chunk chunk)
{
	m_chunksUMap[chunk.GetCoord()] = &chunk;
}

static unsigned int calculateChunksDistance(ChunkCoord coord1, ChunkCoord coord2)
{
	/* Return the distance between the two chunks */
	return (unsigned int)(glm::abs(coord1.idx - coord2.idx) + glm::abs(coord1.idz - coord2.idz));
}

const float* Map::GetVertexBufferToRender(ChunkCoord chunkPlayerPosition)
{
	std::vector<float> vertexBuffer;
	for (std::pair<ChunkCoord, Chunk*> element : m_chunksUMap)
	{
		/* If in render distance, asks for the chunk's vertexBuffer and add it to the global vertexBuffer */
		if (calculateChunksDistance(element.first, chunkPlayerPosition) <= RENDER_DISTANCE)
		{
			std::vector<float> chunkVertexBuffer = element.second->GetVertexBufferToRender();
			vertexBuffer.insert(vertexBuffer.end(), chunkVertexBuffer.begin(), chunkVertexBuffer.end());
		}
	}

	return &vertexBuffer[0];
}
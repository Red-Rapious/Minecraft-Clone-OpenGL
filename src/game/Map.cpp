#include "Chunk.hpp"
#include "Map.hpp"
#include "glm/glm.hpp"
#include <vector>
#include <iostream>

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
	m_worldVertexBuffer.push_back(0.0f);

	for (std::pair<ChunkCoord, std::unique_ptr<Chunk>> element : m_chunksUMap)
	{
		// If in render distance, asks for the chunk's vertexBuffer and add it to the global vertexBuffer
		if (calculateChunksDistance(element.first, chunkPlayerPosition) <= RENDER_DISTANCE)
		{
			std::cout << "end " << element.second->m_vertexBuffer.size() << "\n";
			
			std::vector<float>* chunkVertexBuffer = element.second->GetVertexBufferToRender();
			m_worldVertexBuffer.insert(m_worldVertexBuffer.end(), (*chunkVertexBuffer).begin(), (*chunkVertexBuffer).end());
		}
	}

	m_vertexCount = m_worldVertexBuffer.size();
	return &m_worldVertexBuffer[0];
}
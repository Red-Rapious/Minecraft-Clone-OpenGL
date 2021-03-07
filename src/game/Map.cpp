#include "Chunk.hpp"
#include "Map.hpp"
#include "glm/glm.hpp"
#include <vector>
#include <iostream>

void Map::AddChunkToMap(Chunk chunk)
{
	unsigned int s = m_chunkVector.size();
	m_chunkVector.push_back(chunk);
	m_chunksUMap[chunk.GetCoord()] = &m_chunkVector[s];
}

static unsigned int calculateChunksDistance(ChunkCoord coord1, ChunkCoord coord2)
{
	/* Return the distance between the two chunks */
	return (unsigned int)(glm::abs(coord1.idx - coord2.idx) + glm::abs(coord1.idz - coord2.idz));
}

std::vector<float> Map::GetVertexBufferToRender(ChunkCoord chunkPlayerPosition)
{
	//for (std::pair<ChunkCoord, Chunk*> element : m_chunksUMap)
	for (unsigned int i = 0 ; i < m_chunkVector.size() ; i++)
	{
		// If in render distance, asks for the chunk's vertexBuffer and add it to the global vertexBuffer
		if (calculateChunksDistance(m_chunkVector[i].GetCoord(), chunkPlayerPosition) <= RENDER_DISTANCE)
		{
			
			std::vector<float>* chunkVertexBuffer = m_chunkVector[i].GetVertexBufferToRender(false);
			
			m_worldVertexBuffer.insert(m_worldVertexBuffer.end(), (*chunkVertexBuffer).begin(), (*chunkVertexBuffer).end());
		}
	}

	if (m_worldVertexBuffer.size() == 0)
		std::cout << "[VertexBufferRenderingError] The vector containing vertices is empty, cannot convert it to an array.\n";
	return m_worldVertexBuffer;
}
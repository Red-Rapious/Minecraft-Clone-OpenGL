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

VertexIndexBufferCouple Map::GetCoupleToRender(ChunkCoord chunkPlayerPosition)
{
	m_worldCouple = VertexIndexBufferCouple();
	for (unsigned int i = 0 ; i < m_chunkVector.size() ; i++)
	{
		// If in render distance, asks for the chunk's vertexBuffer and add it to the global vertexBuffer
		if (calculateChunksDistance(m_chunkVector[i].GetCoord(), chunkPlayerPosition) <= RENDER_DISTANCE)
		{
			VertexIndexBufferCouple chunkCouple = m_chunkVector[i].GetCoupleToRender(m_worldCouple.m_indexCount, false);
			m_worldCouple += chunkCouple;
		}
	}

	if (m_worldCouple.m_vertexBuffer.size() == 0)
		std::cout << "[VertexBufferRenderingError] The vector containing vertices is empty, cannot convert it to an array.\n";
	return m_worldCouple;
}
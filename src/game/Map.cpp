#include "Chunk.hpp"
#include "Map.hpp"
#include "glm/glm.hpp"
#include <vector>
#include <iostream>
#include <memory>
#include <unordered_map>

ChunkCoord Map::ConvertPositionToChunkCoord(const glm::vec3& position)
{
	return ChunkCoord(position.x / CHUNK_X_BLOCK_COUNT, position.z / CHUNK_Z_BLOCK_COUNT);
}

std::vector<ChunkCoord> Map::GetChunksCoordsToRender()
{
	//if (calculateChunksDistance(chunkCoordVector[i].GetCoord(), chunkPlayerPosition) <= RENDER_DISTANCE)
	std::vector<ChunkCoord> vect;
	vect.push_back(ChunkCoord(0, 0));
	vect.push_back(ChunkCoord(1, 0));
	return vect;
}

void Map::AddChunkToMap(const Chunk& chunk)
{
	//m_chunkVector.push_back(chunk);
	m_chunksUMap[chunk.GetCoord()] = std::make_unique<Chunk>(chunk);
}

static unsigned int calculateChunksDistance(ChunkCoord coord1, ChunkCoord coord2)
{
	/* Return the manhattan distance between the two chunks */
	return (unsigned int)(glm::abs(coord1.idx - coord2.idx) + glm::abs(coord1.idz - coord2.idz));
}

VertexIndexBufferCouple Map::GetCoupleToRender(ChunkCoord chunkPlayerPosition)
{
	m_worldCouple = VertexIndexBufferCouple();
	std::vector<ChunkCoord> chunkCoordVector = GetChunksCoordsToRender();
	for (unsigned int i = 0 ; i < chunkCoordVector.size() ; i++)
	{
		// If the chunk exists, render it
		if (m_chunksUMap.find(chunkCoordVector[i]) != m_chunksUMap.end())
		{
			VertexIndexBufferCouple chunkCouple = m_chunksUMap.at(chunkCoordVector[i])->GetCoupleToRender(m_worldCouple.m_indexCount, m_chunksUMap);
			m_worldCouple += chunkCouple;
		}
	}

	if (m_worldCouple.m_vertexBuffer.size() == 0)
		std::cout << "[VertexBufferRenderingError] The vector containing vertices is empty, cannot convert it to an array.\n";
	return m_worldCouple;
}

void Map::UpdateChunkGeneration(const glm::vec3& cameraPosition)
{
	m_playerPosition = ConvertPositionToChunkCoord(cameraPosition);
}
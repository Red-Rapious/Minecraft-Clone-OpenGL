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
	std::vector<ChunkCoord> chunksCoordToRender;
	for (int i = -RENDER_DISTANCE / 2 + m_playerPosition.idx; i < RENDER_DISTANCE / 2 + m_playerPosition.idx; i++)
	{
		for (int j = -RENDER_DISTANCE / 2 + m_playerPosition.idz; j < RENDER_DISTANCE / 2 + m_playerPosition.idz; j++)
		{
			if (m_chunksUMap.find(ChunkCoord(i, j)) != m_chunksUMap.end()) // if the chunk exists
				chunksCoordToRender.push_back(ChunkCoord(i, j));
			/*else
				// TODO: Add the chunk to render queue*/
		}
	}
	return chunksCoordToRender;
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

void Map::UpdatePlayerPosition(const glm::vec3& cameraPosition)
{
	ChunkCoord playerPosition = ConvertPositionToChunkCoord(cameraPosition);
	if (!(m_playerPosition == playerPosition))
	{
		m_playerPosition = playerPosition;
		UpdateChunkGeneration();
	}
}

void Map::UpdateChunkGeneration()
{
	
}
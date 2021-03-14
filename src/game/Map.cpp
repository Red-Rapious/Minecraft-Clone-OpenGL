#include "Chunk.hpp"
#include "Map.hpp"
#include "glm/glm.hpp"
#include <vector>
#include <iostream>
#include <memory>
#include <unordered_map>

void Map::AddChunkToGenQueue(const ChunkCoord& chunkCoord)
{
	if (std::find(m_chunkGenerationQueue.begin(), m_chunkGenerationQueue.end(), chunkCoord) == m_chunkGenerationQueue.end())
		m_chunkGenerationQueue.push_back(chunkCoord);
}

ChunkCoord Map::ConvertPositionToChunkCoord(const glm::vec3& position)
{
	int x = position.z / CHUNK_Z_BLOCK_COUNT; // invert because of some strange bug
	int z = position.x / CHUNK_X_BLOCK_COUNT;

	// -1/2 == 0, so to make the negative coords work we need to fix this
	if (position.z < 0)
		x -= 1;
	if (position.x < 0)
		z -= 1;

	return ChunkCoord(x,z);
}

void Map::RenderSpecificChunk(const ChunkCoord& coord, std::vector<ChunkCoord>& chunksCoordToRender)
{
	// TODO: separate render from generation
	if (m_chunksUMap.find(coord) != m_chunksUMap.end()) // if the chunk exists
		chunksCoordToRender.push_back(coord);
	else
		AddChunkToGenQueue(coord);
}

std::vector<ChunkCoord> Map::GetChunksCoordsToRender()
{
	// Render the chunks like rings starting from the center
	std::vector<ChunkCoord> chunksCoordToRender;

	RenderSpecificChunk(ChunkCoord(m_playerPosition.idx, m_playerPosition.idz), chunksCoordToRender);
	for (int d = 1; d < RENDER_DISTANCE ; d++)
	{
		for (int x = -d ; x <= d ; x++)
		{
			if (x == -d || x == d)
			{
				for (int z = -d ; z <= d ; z++)
				{
					RenderSpecificChunk(ChunkCoord(x + m_playerPosition.idx, z + m_playerPosition.idz), chunksCoordToRender);
				}
			}
			else
			{
				RenderSpecificChunk(ChunkCoord(x + m_playerPosition.idx, -d + m_playerPosition.idz), chunksCoordToRender);
				RenderSpecificChunk(ChunkCoord(x + m_playerPosition.idx, d + m_playerPosition.idz), chunksCoordToRender);
			}
		}
	}

	return chunksCoordToRender;
}

void Map::AddChunkToMap(const Chunk& chunk)
{
	m_chunksUMap[chunk.GetCoord()] = std::make_unique<Chunk>(chunk);
}

void Map::RenderAllNeededChunks(const VertexArray& vao)
{
	std::vector<ChunkCoord> chunkCoordVector = GetChunksCoordsToRender();
	if (chunkCoordVector.size() == 0)
	{
		//std::cout << "[ChunksToRender Warning] No chunk to render, can't render all needed chunks\n";
	}
	else
	{
		for (unsigned int i = 0; i < chunkCoordVector.size(); i++)
		{
			// If the chunk exists, render it
			if (m_chunksUMap.find(chunkCoordVector[i]) != m_chunksUMap.end())
			{
				m_chunksUMap.at(chunkCoordVector[i])->RenderChunk(vao, m_chunksUMap);
				
			}
		}
	}
}

unsigned int Map::calculateChunksDistance(const ChunkCoord& coord1, const ChunkCoord& coord2)
{
	/* Return the manhattan distance between the two chunks */
	return (unsigned int)(glm::abs(coord1.idx - coord2.idx) + glm::abs(coord1.idz - coord2.idz));
}


void Map::UpdateChunkPlayerPosition(const glm::vec3& cameraPosition)
{
	ChunkCoord playerPosition = ConvertPositionToChunkCoord(cameraPosition);
	if (!(m_playerPosition == playerPosition))
	{
		m_playerPosition = playerPosition;
	}
}

bool Map::GenerateOneChunk()
{
	if (m_chunkGenerationQueue.size() == 0)
		return false; // no new chunk generated
	
	ChunkCoord generatedChunkCoord = m_chunkGenerationQueue[0];
	Chunk generatedChunk(generatedChunkCoord);
	
	AddChunkToMap(generatedChunk);
	m_chunksUMap.at(generatedChunkCoord)->Generate();
	m_chunksUMap.at(generatedChunkCoord)->ListAllFacesToRender(m_chunksUMap);

	m_chunkGenerationQueue.erase(m_chunkGenerationQueue.begin()); // delete the chunk from the render queue
	return true; // a new chunk was generated
}
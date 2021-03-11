#include "Chunk.hpp"
#include "Map.hpp"
#include "glm/glm.hpp"
#include <vector>
#include <iostream>
#include <memory>
#include <unordered_map>

void Map::AddChunkToGenQueue(ChunkCoord chunkCoord)
{
	if (std::find(m_chunkGenerationQueue.begin(), m_chunkGenerationQueue.end(), chunkCoord) == m_chunkGenerationQueue.end())
		m_chunkGenerationQueue.push_back(chunkCoord);
}

ChunkCoord Map::ConvertPositionToChunkCoord(const glm::vec3& position)
{
	return ChunkCoord(position.x / CHUNK_X_BLOCK_COUNT, position.z / CHUNK_Z_BLOCK_COUNT);
}

void Map::RenderChunk(const ChunkCoord& coord, std::vector<ChunkCoord>& chunksCoordToRender)
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
	for (int d = 1; d < RENDER_DISTANCE+4; d+=2)
	{
		for (int x = -d/2; x < d/2+1; x++)
		{
			if (x == -d / 2 || x == d / 2)
			{
				for (int z = -d / 2; z < d / 2 + 1; z++)
				{
					RenderChunk(ChunkCoord(x+ m_playerPosition.idx, z+ m_playerPosition.idz), chunksCoordToRender);
				}
			}
			else
			{
				RenderChunk(ChunkCoord(x + m_playerPosition.idx, -d/2 + m_playerPosition.idz), chunksCoordToRender);
				RenderChunk(ChunkCoord(x + m_playerPosition.idx, d/2 + m_playerPosition.idz), chunksCoordToRender);
			}
		}
	}
	
	return chunksCoordToRender;
}

void Map::AddChunkToMap(const Chunk& chunk)
{
	m_chunksUMap[chunk.GetCoord()] = std::make_unique<Chunk>(chunk);
}

void Map::RenderAllNeededChunks(VertexArray& vao, const Renderer& renderer)
{
	// Called by main (for now)
	// Takes the coords of all the chunks needed to render, and asks the chunk to render itself
	std::vector<ChunkCoord> chunkCoordVector = GetChunksCoordsToRender();
	//unsigned int lastIndexCount = 0;

	

	for (unsigned int i = 0; i < chunkCoordVector.size(); i++)
	{
		// If the chunk exists, render it
		if (m_chunksUMap.find(chunkCoordVector[i]) != m_chunksUMap.end())
		{
			m_chunksUMap.at(chunkCoordVector[i])->UpdateCoupleToRender(m_chunksUMap);
			m_chunksUMap.at(chunkCoordVector[i])->RenderChunk(renderer, vao);
		}
	}

	
}

static unsigned int calculateChunksDistance(ChunkCoord coord1, ChunkCoord coord2)
{
	/* Return the manhattan distance between the two chunks */
	return (unsigned int)(glm::abs(coord1.idx - coord2.idx) + glm::abs(coord1.idz - coord2.idz));
}


void Map::UpdatePlayerPosition(const glm::vec3& cameraPosition)
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
	generatedChunk.Generate();
	AddChunkToMap(generatedChunk);
	m_chunksUMap.at(generatedChunkCoord)->UpdateCoupleToRender(m_chunksUMap);
	m_chunkGenerationQueue.erase(m_chunkGenerationQueue.begin()); // delete the chunk from the render queue
	return true; // a new chunk was generated
}
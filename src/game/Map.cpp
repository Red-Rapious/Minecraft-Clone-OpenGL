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

	/*for (int i = -RENDER_DISTANCE / 2 + m_playerPosition.idx; i < RENDER_DISTANCE / 2 + m_playerPosition.idx; i++)
	{
		for (int j = -RENDER_DISTANCE / 2 + m_playerPosition.idz; j < RENDER_DISTANCE / 2 + m_playerPosition.idz; j++)
		{
			RenderChunk(ChunkCoord(i,j), chunksCoordToRender);
		}
	}*/

	RenderChunk(ChunkCoord(m_playerPosition.idz, m_playerPosition.idx), chunksCoordToRender);

	/*for (int d = 1; d < RENDER_DISTANCE+4; d+=2)
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
	}*/
	
	//std::cout << chunksCoordToRender.size() << "\n";
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
	
	AddChunkToMap(generatedChunk);
	m_chunksUMap.at(generatedChunkCoord)->Generate();
	m_chunksUMap.at(generatedChunkCoord)->GetCoupleToRender(m_chunksUMap);

	m_chunkGenerationQueue.erase(m_chunkGenerationQueue.begin()); // delete the chunk from the render queue
	return true; // a new chunk was generated
}
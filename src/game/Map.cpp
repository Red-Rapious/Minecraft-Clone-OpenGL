#include "Chunk.hpp"
#include "Map.hpp"
#include "glm/glm.hpp"
#include <vector>
#include <iostream>
#include <memory>
#include <unordered_map>

Map::Map()
	: m_playerPosition(ChunkCoord(0, 0)), m_chunkGenerationQueue({}), m_noise(std::random_device()), m_seed(1234), m_lastChunksToRender({})
{
	m_noise.reseed((uint32_t)m_seed);
}

void Map::AddChunkToGenQueue(const ChunkCoord& chunkCoord)
{
	if (std::find(m_chunkGenerationQueue.begin(), m_chunkGenerationQueue.end(), chunkCoord) == m_chunkGenerationQueue.end())
		m_chunkGenerationQueue.push_back(chunkCoord);
}

ChunkCoord Map::ConvertPositionToChunkCoord(const glm::vec3& position)
{	// Convert a 3D position into a 2D, chunk-normalized position
	int x = position.x / CHUNK_X_BLOCK_COUNT;
	int z = position.z / CHUNK_Z_BLOCK_COUNT;

	// -1/2 == 0, so to make the negative coords work we need to fix this
	if (position.z < 0)
		z -= 1;
	if (position.x < 0)
		x -= 1;

	return ChunkCoord(x,z);
}

void Map::RenderSpecificChunk(const ChunkCoord& coord, std::vector<ChunkCoord>& chunksCoordToRender)
{	// Add the chunk to the render list if its generated, otherwise add it to the generation queue
	if (m_chunksUMap.find(coord) != m_chunksUMap.end()) // if the chunk exists
	{
		chunksCoordToRender.push_back(coord);
		if (!(std::count(m_lastChunksToRender.begin(), m_lastChunksToRender.end(), coord)))// if the chunk wasn't in the last frame, generate his buffers
			m_chunksUMap.at(coord)->GenerateBuffers();
	}
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

	for (unsigned int i = 0; i < m_lastChunksToRender.size(); i++) 
		// for every chunk who was there last frame but is not in the actual one
	{
		if (!(std::count(chunksCoordToRender.begin(), chunksCoordToRender.end(), m_lastChunksToRender[i]))) // if isn't rendered this frame
			GetChunkByCoord(m_lastChunksToRender[i])->DeleteBuffers(); // free the buffers from memory to prevent memory leaks
	}

	m_lastChunksToRender = chunksCoordToRender;
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

void Map::UpdateChunkPlayerPosition(const glm::vec3& cameraPosition)
{
	ChunkCoord playerPosition = ConvertPositionToChunkCoord(cameraPosition);
	if (!(m_playerPosition == playerPosition))
	{
		m_playerPosition = playerPosition;
	}
}

bool Map::GenerateOneChunk()
{	// Function called every frame: generate one chunk from the generation queue
	if (m_chunkGenerationQueue.size() == 0)
		return false; // no new chunk generated
	
	// else: if there's chunks to generate
	ChunkCoord generatedChunkCoord = m_chunkGenerationQueue[0];
	Chunk generatedChunk(generatedChunkCoord);
	
	AddChunkToMap(generatedChunk);
	m_chunksUMap.at(generatedChunkCoord)->Generate(m_noise, m_chunksUMap);
	m_chunksUMap.at(generatedChunkCoord)->ListAllFacesToRender(m_chunksUMap);

	m_chunkGenerationQueue.erase(m_chunkGenerationQueue.begin()); // delete the chunk from the render queue
	return true; // a new chunk was generated
}
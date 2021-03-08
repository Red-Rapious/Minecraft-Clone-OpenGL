#include "Chunk.hpp"
#include "Constants.hpp"
#include <iostream>
#include <vector>

/* VERTEX AND INDEX BUFFERS PART */
static float getTextureOriginY(BlockType blockType)
{
	return (BLOCK_TYPES_TEXTURES_NUMBER - (int)(blockType));
}

static float getTextureCoordX(FaceType faceType)
{
	return ((int)faceType);
}

void Chunk::ClearVertexBuffer()
{
	m_vertexIndexBufferCouple.m_vertexBuffer = {};
}

void Chunk::AddVertexToVertexBuffer(glm::vec3 vertexCoord, glm::vec2 textureCoord)
{
	m_vertexIndexBufferCouple.m_vertexBuffer.push_back((float)vertexCoord.x);
	m_vertexIndexBufferCouple.m_vertexBuffer.push_back((float)vertexCoord.y);
	m_vertexIndexBufferCouple.m_vertexBuffer.push_back((float)vertexCoord.z);

	m_vertexIndexBufferCouple.m_vertexBuffer.push_back((float)textureCoord.x);
	m_vertexIndexBufferCouple.m_vertexBuffer.push_back((float)textureCoord.y);
}


void Chunk::AddFaceToCouple(FaceType faceType, glm::vec3 blockCoord, BlockType blockType)
{
	glm::vec3 vertexCoord[4] = {}; // one for each vertex of each face
	glm::vec2 textureCoord[4] = {};
	unsigned int indices[6] = { 0,1,2,0,2,3 };

	switch (faceType)
	{
	default:
		break;
	case FaceType::FRONT:
		vertexCoord[0] = glm::vec3(-0.5f, -0.5f, 0.5f); // D
		vertexCoord[1] = glm::vec3(-0.5f, -0.5f, -0.5f); // A
		vertexCoord[2] = glm::vec3(-0.5f, 0.5f, -0.5f); // H
		vertexCoord[3] = glm::vec3(-0.5f, 0.5f, 0.5f); // E


		textureCoord[0] = glm::vec2(1, 0); // D
		textureCoord[1] = glm::vec2(0, 0); // A
		textureCoord[2] = glm::vec2(0, 1); // H
		textureCoord[3] = glm::vec2(1, 1); // E
		break;
		
	case FaceType::BACK:
		vertexCoord[0] = glm::vec3(0.5f, -0.5f, -0.5f); // B
		vertexCoord[1] = glm::vec3(0.5f, -0.5f, 0.5f); // C
		vertexCoord[2] = glm::vec3(0.5f, 0.5f, 0.5f); // F
		vertexCoord[3] = glm::vec3(0.5f, 0.5f, -0.5f); // G


		textureCoord[0] = glm::vec2(1, 0); // B
		textureCoord[1] = glm::vec2(0, 0); // C
		textureCoord[2] = glm::vec2(0, 1); // F
		textureCoord[3] = glm::vec2(1, 1); // G
		break;
		
		
	
	case FaceType::LEFT:
		vertexCoord[0] = glm::vec3(-0.5f, -0.5f, -0.5f); // A
		vertexCoord[1] = glm::vec3(0.5f, -0.5f, -0.5f); // B
		vertexCoord[2] = glm::vec3(0.5f, 0.5f, -0.5f); // G
		vertexCoord[3] = glm::vec3(-0.5f, 0.5f, -0.5f); // H


		textureCoord[0] = glm::vec2(1, 0); // A
		textureCoord[1] = glm::vec2(0, 0); // B
		textureCoord[2] = glm::vec2(0, 1); // G
		textureCoord[3] = glm::vec2(1, 1); // H
		break;
		

	case FaceType::RIGHT:
		vertexCoord[0] = glm::vec3(0.5f, -0.5f, 0.5f); // C
		vertexCoord[1] = glm::vec3(-0.5f, -0.5f, 0.5f); // D
		vertexCoord[2] = glm::vec3(-0.5f, 0.5f, 0.5f); // E
		vertexCoord[3] = glm::vec3(0.5f, 0.5f, 0.5f); // F


		textureCoord[0] = glm::vec2(1, 0); // C
		textureCoord[1] = glm::vec2(0, 0); // D
		textureCoord[2] = glm::vec2(0, 1); // E
		textureCoord[3] = glm::vec2(1, 1); // F
		break;
		

	case FaceType::UP:
		vertexCoord[0] = glm::vec3(-0.5f, 0.5f, -0.5f); // H
		vertexCoord[1] = glm::vec3(0.5f, 0.5f, -0.5f); // G
		vertexCoord[2] = glm::vec3(0.5f, 0.5f, 0.5f); // F
		vertexCoord[3] = glm::vec3(-0.5f, 0.5f, 0.5f); // E


		textureCoord[0] = glm::vec2(0, 0); // H
		textureCoord[1] = glm::vec2(0, 1); // G
		textureCoord[2] = glm::vec2(1, 1); // F
		textureCoord[3] = glm::vec2(1, 0); // E
		break;

	case FaceType::DOWN:
		vertexCoord[0] = glm::vec3(0.5f, -0.5f, -0.5f); // B
		vertexCoord[1] = glm::vec3(-0.5f, -0.5f, -0.5f); // A
		vertexCoord[2] = glm::vec3(-0.5f, -0.5f, 0.5f); // D
		vertexCoord[3] = glm::vec3(0.5f, -0.5f, 0.5f); // C


		textureCoord[0] = glm::vec2(0, 0); // B
		textureCoord[1] = glm::vec2(0, 1); // A
		textureCoord[2] = glm::vec2(1, 1); // D
		textureCoord[3] = glm::vec2(1, 0); // C
		break;
	}



	float textureOriginX = 0.0f;
	float textureOriginY = 0.0f;
	for (int i = 0; i < sizeof(vertexCoord) / sizeof(vertexCoord[0]); i++)
	{
		// Add the block coord (in world space) to the vertex
		vertexCoord[i].x += blockCoord.z; // reverse to fix a coordinates bug
		vertexCoord[i].y += blockCoord.y;
		vertexCoord[i].z += blockCoord.x;
		
		vertexCoord[i] += glm::vec3(m_coord.idz * CHUNK_Z_BLOCK_COUNT, 0, m_coord.idx * CHUNK_X_BLOCK_COUNT); // chunk coord in world space
		
		// Add the texture origin on the texture map
		textureOriginY = getTextureOriginY(blockType); // TODO: dont compute it for every face, only for every block
		textureOriginX = getTextureCoordX(faceType);
		textureCoord[i].x = (textureOriginX + textureCoord[i].x) / 6;
		textureCoord[i].y = (textureOriginY + textureCoord[i].y) / BLOCK_TYPES_TEXTURES_NUMBER;

		// Add vertex to the vertex buffer
		AddVertexToVertexBuffer(vertexCoord[i], textureCoord[i]);
	}

	// Add the good indices to the index buffer
	for (unsigned int i = 0; i < 6; i++)
	{
		m_vertexIndexBufferCouple.m_indexBuffer.push_back(indices[i]+ m_vertexIndexBufferCouple.m_indexCount);
		
	}
	m_vertexIndexBufferCouple.m_indexCount+=4;
}



/* STANDARD FUNCTIONS PART */
Chunk::Chunk(ChunkCoord coord)
	: m_coord(coord), m_blocksArray(), m_vertexIndexBufferCouple()
{
	// Initialise the chunk to an empty cube of air
	DeleteAllBlocks();
}

void Chunk::SetBlockType(glm::vec3 blockPosition, BlockType type)
{
	if (!(blockPosition.x >= 0 && blockPosition.x < CHUNK_X_BLOCK_COUNT && blockPosition.y >= 0 && blockPosition.y < CHUNK_Y_BLOCK_COUNT && blockPosition.z >= 0 && blockPosition.z < CHUNK_Z_BLOCK_COUNT))
		std::cout << "[SetBlockType] Impossible to set this block, the coordinates are incorrect\n";
	else
		m_blocksArray[(unsigned int)blockPosition.x][(unsigned int)blockPosition.y][(unsigned int)blockPosition.z] = type;
}


void Chunk::FillPlaneWithBlocks(unsigned int height, BlockType type)
{
	for (unsigned int x = 0; x < CHUNK_X_BLOCK_COUNT ; x++)
	{
		for (unsigned int z = 0 ; z < CHUNK_Z_BLOCK_COUNT; z++)
		{
			m_blocksArray[x][height][z] = type;
		}
	}

}

void Chunk::DeleteAllBlocks()
{
	m_blocksArray = {};
	for (unsigned int x = 0; x < CHUNK_X_BLOCK_COUNT; x++)
	{
		std::vector<std::vector<BlockType>> vect;
		m_blocksArray.push_back(vect);
		for (unsigned int y = 0; y < CHUNK_Y_BLOCK_COUNT; y++)
		{
			std::vector<BlockType> vect;
			m_blocksArray[x].push_back(vect);
			for (unsigned int z = 0; z < CHUNK_Z_BLOCK_COUNT; z++)
			{
				m_blocksArray[x][y].push_back(BlockType::NONE);
			}
		}
	}
}

unsigned int Chunk::GetNumberOfNonAirBlocks(const bool& out) const
{
	if (m_blocksArray.size() == 0)
	{
		std::cout << "[Chunk Blocks Error] The array of blocks is empty, cannot calculate any blocks\n";
		return 0;
	}
	/* Debug func that count the number of "true" blocks */
	unsigned int count = 0;

	for (unsigned int x = 0; x < CHUNK_X_BLOCK_COUNT; x++)
	{
		for (unsigned int y = 0; y < CHUNK_Y_BLOCK_COUNT; y++)
		{
			for (unsigned int z = 0; z < CHUNK_Z_BLOCK_COUNT; z++)
			{
				if (m_blocksArray[x][y][z] != BlockType::NONE)
					count++;
			}
		}
	}

	if (out)
		std::cout << "Number of non air blocks: " << count << "\n";

	return count;
}




/* FACE TO RENDER ALGORITHM PART */
VertexIndexBufferCouple Chunk::GetCoupleToRender(const unsigned int& originIndex, const std::unordered_map<ChunkCoord, Chunk*, ChunkCoordHash>& chunksUMap)
{
	m_vertexIndexBufferCouple.m_indexCount = originIndex;
	RenderAllFacesNeeded(chunksUMap);
	//UpdateInsideCoupleToRender();
	//UpdateOutsideCoupleToRender(false, false, false, false);
	return m_vertexIndexBufferCouple;
}

/*
void Chunk::UpdateOutsideCoupleToRender(const bool& chunkNorth, const bool& chunkSouth, const bool& chunkWest, const bool& chunkEast)
{
	// Check if there's a block up and down the block
	for (unsigned int y = 0; y < CHUNK_Y_BLOCK_COUNT; y++)
	{
		for (unsigned int x = 0; x < CHUNK_X_BLOCK_COUNT; x++)
		{
			// SOUTH
			if (m_blocksArray[x][y][0] != BlockType::NONE)
			{
				if (y==0)
					AddFaceToCouple(FaceType::DOWN, glm::vec3(x, y, 0), m_blocksArray[x][y][0]);

				else if (m_blocksArray[x][y - 1][0] == BlockType::NONE)
					AddFaceToCouple(FaceType::DOWN, glm::vec3(x, y, 0), m_blocksArray[x][y][0]);

				if (y == CHUNK_Y_BLOCK_COUNT - 1)
					AddFaceToCouple(FaceType::UP, glm::vec3(x, y, 0), m_blocksArray[x][y][0]);

				else if (m_blocksArray[x][y + 1][0] == BlockType::NONE)
					AddFaceToCouple(FaceType::UP, glm::vec3(x, y, 0), m_blocksArray[x][y][0]);
			}

			// NORTH
			if (m_blocksArray[x][y][CHUNK_Z_BLOCK_COUNT-1] != BlockType::NONE)
			{
				if (y == CHUNK_Y_BLOCK_COUNT-1)
					AddFaceToCouple(FaceType::UP, glm::vec3(x, y, CHUNK_Z_BLOCK_COUNT - 1), m_blocksArray[x][y][CHUNK_Z_BLOCK_COUNT - 1]);

				else if (m_blocksArray[x][y + 1][CHUNK_Z_BLOCK_COUNT - 1] == BlockType::NONE)
					AddFaceToCouple(FaceType::UP, glm::vec3(x, y, CHUNK_Z_BLOCK_COUNT - 1), m_blocksArray[x][y][CHUNK_Z_BLOCK_COUNT - 1]);

				if (y == 0)
					AddFaceToCouple(FaceType::DOWN, glm::vec3(x, y, 0), m_blocksArray[x][y][0]);
				
				else if (m_blocksArray[x][y - 1][CHUNK_Z_BLOCK_COUNT - 1] == BlockType::NONE)
					AddFaceToCouple(FaceType::DOWN, glm::vec3(x, y, CHUNK_Z_BLOCK_COUNT - 1), m_blocksArray[x][y][CHUNK_Z_BLOCK_COUNT - 1]);

				
			}
				
		}


		for (unsigned int z = 0; z < CHUNK_Z_BLOCK_COUNT; z++)
		{
			// SOUTH
			if (m_blocksArray[0][y][z] != BlockType::NONE)
			{
				if (y == 0)
					AddFaceToCouple(FaceType::DOWN, glm::vec3(0, y, z), m_blocksArray[0][y][z]);

				else if (m_blocksArray[0][y - 1][z] == BlockType::NONE)
					AddFaceToCouple(FaceType::DOWN, glm::vec3(0, y, z), m_blocksArray[0][y][z]);
				
				if (y == CHUNK_Y_BLOCK_COUNT - 1)
					AddFaceToCouple(FaceType::UP, glm::vec3(0, y, z), m_blocksArray[0][y][z]);

				else if (m_blocksArray[0][y + 1][z] == BlockType::NONE)
					AddFaceToCouple(FaceType::UP, glm::vec3(0, y, z), m_blocksArray[0][y][z]);
			}

			// NORTH
			if (m_blocksArray[CHUNK_X_BLOCK_COUNT - 1][y][z] != BlockType::NONE)
			{
				if (y == CHUNK_Y_BLOCK_COUNT - 1)
					AddFaceToCouple(FaceType::UP, glm::vec3(CHUNK_X_BLOCK_COUNT - 1, y, z), m_blocksArray[CHUNK_X_BLOCK_COUNT - 1][y][z]);
				
				else if (m_blocksArray[CHUNK_X_BLOCK_COUNT - 1][y + 1][CHUNK_Z_BLOCK_COUNT - 1] == BlockType::NONE)
					AddFaceToCouple(FaceType::UP, glm::vec3(CHUNK_X_BLOCK_COUNT - 1, y, z), m_blocksArray[CHUNK_X_BLOCK_COUNT - 1][y][z]);

				if (y == 0)
					AddFaceToCouple(FaceType::DOWN, glm::vec3(CHUNK_X_BLOCK_COUNT - 1, y, z), m_blocksArray[CHUNK_X_BLOCK_COUNT - 1][y][z]);
				
				else if (m_blocksArray[CHUNK_X_BLOCK_COUNT - 1][y - 1][z] == BlockType::NONE)
					AddFaceToCouple(FaceType::DOWN, glm::vec3(CHUNK_X_BLOCK_COUNT - 1, y, z), m_blocksArray[CHUNK_X_BLOCK_COUNT - 1][y][z]);

				}

		}
	}


	// For each border block, render the faces depending of the chunks around
	if (chunkSouth)
	{
		// TODO: check on the map if there's a block sticked to it
	}
	else
	{
		for (unsigned int x = 0; x < CHUNK_X_BLOCK_COUNT; x++)
		{
			for (unsigned int y = 0; y < CHUNK_Y_BLOCK_COUNT; y++)
			{
				if (m_blocksArray[x][y][0]!=BlockType::NONE)
					AddFaceToCouple(FaceType::FRONT, glm::vec3(x,y,0), m_blocksArray[x][y][0]);

			}
		}
	}

	if (chunkNorth)
	{
		// TODO: check on the map if there's a block sticked to it
	}
	else
	{
		for (unsigned int x = 0; x < CHUNK_X_BLOCK_COUNT; x++)
		{
			for (unsigned int y = 0; y < CHUNK_Y_BLOCK_COUNT; y++)
			{
				if (m_blocksArray[x][y][CHUNK_Z_BLOCK_COUNT - 1] != BlockType::NONE)
					AddFaceToCouple(FaceType::BACK, glm::vec3(x, y, CHUNK_Z_BLOCK_COUNT-1), m_blocksArray[x][y][CHUNK_Z_BLOCK_COUNT - 1]);
			}
		}
	}

	if (chunkWest)
	{
		// TODO: check on the map if there's a block sticked to it
	}
	else
	{
		for (unsigned int z = 0; z < CHUNK_Z_BLOCK_COUNT; z++)
		{
			for (unsigned int y = 0; y < CHUNK_Y_BLOCK_COUNT; y++)
			{
				if (m_blocksArray[0][y][z] != BlockType::NONE)
					AddFaceToCouple(FaceType::LEFT, glm::vec3(0, y, z), m_blocksArray[0][y][z]);
			}
		}
	}

	if (chunkEast)
	{
		// TODO: check on the map if there's a block sticked to it
	}
	else
	{
		for (unsigned int z = 0; z < CHUNK_Z_BLOCK_COUNT; z++)
		{
			for (unsigned int y = 0; y < CHUNK_Y_BLOCK_COUNT; y++)
			{
				if (m_blocksArray[CHUNK_X_BLOCK_COUNT - 1][y][z] != BlockType::NONE)
					AddFaceToCouple(FaceType::RIGHT, glm::vec3(CHUNK_X_BLOCK_COUNT-1, y, z), m_blocksArray[CHUNK_X_BLOCK_COUNT - 1][y][z]);
			}
		}
	}
}

void Chunk::UpdateInsideCoupleToRender()
{
	ClearVertexBuffer();
	BlockType analysedBlockType;
	
	for (unsigned int x = 1; x < CHUNK_X_BLOCK_COUNT; x++)
	{
		for (unsigned int y = 0; y < CHUNK_Y_BLOCK_COUNT; y++)
		{
			for (unsigned int z = 1; z < CHUNK_Z_BLOCK_COUNT; z++)
			{
				
				analysedBlockType = m_blocksArray[x][y][z];
				if (analysedBlockType != BlockType::NONE && !(x == CHUNK_X_BLOCK_COUNT-1 || y == CHUNK_Y_BLOCK_COUNT-1 || z == CHUNK_Z_BLOCK_COUNT-1)) // else: do not render the face
				{
					glm::vec3 coord(x, y, z);

					// For each face, check if there's a face next to it ; if not, render the face
					// FRONT
					if (m_blocksArray[x][y][z - 1] == BlockType::NONE)
					{
						AddFaceToCouple(FaceType::FRONT, coord, analysedBlockType);
					}

					// BACK
					if (m_blocksArray[x][y][z + 1] == BlockType::NONE)
					{
						AddFaceToCouple(FaceType::BACK, coord, analysedBlockType);
					}

					// UP
					if (y==CHUNK_Y_BLOCK_COUNT-1 || m_blocksArray[x][y+1][z] == BlockType::NONE)
					{
						AddFaceToCouple(FaceType::UP, coord, analysedBlockType);
					}

					// DOWN
					if (y==0 || m_blocksArray[x][y - 1][z] == BlockType::NONE)
					{
						AddFaceToCouple(FaceType::DOWN, coord, analysedBlockType);
					}

					// LEFT
					if (m_blocksArray[x-1][y][z] == BlockType::NONE)
					{
						AddFaceToCouple(FaceType::LEFT, coord, analysedBlockType);
					}

					// RIGHT
					if (m_blocksArray[x+1][y][z] == BlockType::NONE)
					{
						AddFaceToCouple(FaceType::RIGHT, coord, analysedBlockType);
					}
				}
			}
		}
	}
}*/






void Chunk::RenderAllFacesNeeded(const std::unordered_map<ChunkCoord, Chunk*, ChunkCoordHash>& chunksUMap)
{
	const bool RENDER_UNGEN_CHUNKS_FACES = true;
	const bool RENDER_BOTTOM_CHUNK_FACES = true;

	for (unsigned int x = 0; x < CHUNK_X_BLOCK_COUNT; x++)
	{
		for (unsigned int y = 0; y < CHUNK_Y_BLOCK_COUNT; y++)
		{
			for (unsigned int z = 0; z < CHUNK_Z_BLOCK_COUNT; z++)
			{
				// For each block
				if (m_blocksArray[x][y][z] != BlockType::NONE)
				{
					bool renderFace[6] = { false };
					//bool overwriteFace[6] = { false };

					/* For each face, check if its supposed to be rendered */
					// FRONT
					if (z == 0)
					{
						ChunkCoord otherChunkCoord(m_coord.idx, m_coord.idz - 1);
						if (chunksUMap.find(otherChunkCoord) != chunksUMap.end()) // if a chunk exists where the face points towards
						{
							// render the face if the blocks that correspond on the other chunk is empty
							renderFace[(int)FaceType::FRONT] = chunksUMap.at(otherChunkCoord)->GetBlockType(glm::vec3(x, y, CHUNK_Z_BLOCK_COUNT - 2)) == BlockType::NONE;
						}
						else
							renderFace[(int)FaceType::FRONT] = RENDER_UNGEN_CHUNKS_FACES;
							//overwriteFace[(int)FaceType::FRONT] = RENDER_UNGEN_CHUNKS_FACES;
					}
					else
					{
						renderFace[(int)FaceType::FRONT] = m_blocksArray[x][y][z-1] == BlockType::NONE;
					}

					// BACK
					if (z == CHUNK_Z_BLOCK_COUNT-1)
					{
						ChunkCoord otherChunkCoord(m_coord.idx, m_coord.idz + 1);
						if (chunksUMap.find(otherChunkCoord) != chunksUMap.end()) // if a chunk exists where the face points towards
						{
							// render the face if the blocks that correspond on the other chunk is empty
							renderFace[(int)FaceType::BACK] = chunksUMap.at(otherChunkCoord)->GetBlockType(glm::vec3(x, y, 0)) == BlockType::NONE;
						}
						else
							renderFace[(int)FaceType::BACK] = RENDER_UNGEN_CHUNKS_FACES;
							//overwriteFace[(int)FaceType::BACK] = RENDER_UNGEN_CHUNKS_FACES;
					}
					else
					{
						renderFace[(int)FaceType::BACK] = m_blocksArray[x][y][z + 1] == BlockType::NONE;
					}

					// RIGHT
					if (x == CHUNK_X_BLOCK_COUNT-1)
					{
						ChunkCoord otherChunkCoord(m_coord.idx+1, m_coord.idz);
						if (chunksUMap.find(otherChunkCoord) != chunksUMap.end()) // if a chunk exists where the face points towards
						{
							// render the face if the blocks that correspond on the other chunk is empty
							renderFace[(int)FaceType::RIGHT] = (chunksUMap.at(otherChunkCoord)->GetBlockType(glm::vec3(0, y, z)) == BlockType::NONE);
						}
						else
							renderFace[(int)FaceType::RIGHT] = RENDER_UNGEN_CHUNKS_FACES;
							//overwriteFace[(int)FaceType::RIGHT] = RENDER_UNGEN_CHUNKS_FACES;
					}
					else
					{
						renderFace[(int)FaceType::RIGHT] = m_blocksArray[x + 1][y][z] == BlockType::NONE;
					}

					// LEFT
					if (x == 0)
					{
						ChunkCoord otherChunkCoord(m_coord.idx - 1, m_coord.idz);
						if (chunksUMap.find(otherChunkCoord) != chunksUMap.end()) // if a chunk exists where the face points towards
						{
							// render the face if the blocks that correspond on the other chunk is empty
							chunksUMap.at(otherChunkCoord)->GetNumberOfNonAirBlocks(true);
							renderFace[(int)FaceType::LEFT] = chunksUMap.at(otherChunkCoord)->GetBlockType(glm::vec3(0, y, CHUNK_Z_BLOCK_COUNT-2)) == BlockType::NONE;
						}
						else
							renderFace[(int)FaceType::LEFT] = RENDER_UNGEN_CHUNKS_FACES;
							//overwriteFace[(int)FaceType::LEFT] = RENDER_UNGEN_CHUNKS_FACES;
					}
					else
					{
						renderFace[(int)FaceType::RIGHT] = m_blocksArray[x - 1][y][z] == BlockType::NONE;
					}

					// UP
					if (y == CHUNK_Y_BLOCK_COUNT-1)
					{
						//overwriteFace[(int)FaceType::UP] = true;
						renderFace[(int)FaceType::UP] = true;
					}
					else
					{
						renderFace[(int)FaceType::UP] = m_blocksArray[x][y + 1][z] == BlockType::NONE;
					}

					// DOWN
					if (y == 0)
					{
						//overwriteFace[(int)FaceType::DOWN] = RENDER_BOTTOM_CHUNK_FACES;
						renderFace[(int)FaceType::DOWN] = RENDER_BOTTOM_CHUNK_FACES;
					}
					else
					{
						renderFace[(int)FaceType::DOWN] = m_blocksArray[x][y - 1][z] == BlockType::NONE;
					}


					/* Render each face supposed to be rendered */
					for (unsigned int i = 0; i < 6; i++)
					{
						if (renderFace[i])
						{
							AddFaceToCouple((FaceType)i, glm::vec3(x,y,z), m_blocksArray[x][y][z]);
						}
					}
				}
			}
		}
	}
}
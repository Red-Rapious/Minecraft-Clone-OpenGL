#include "Chunk.hpp"
#include "Constants.hpp"
#include <iostream>
#include <vector>

void Chunk::ClearVertexBuffer()
{
	m_vertexBuffer = {};
}

static void addVertexToVertexBuffer(std::vector<float>& vertexBuffer, glm::vec3 vertexCoord, glm::vec2 textureCoord)
{
	vertexBuffer.push_back((float)vertexCoord.x);
	vertexBuffer.push_back((float)vertexCoord.y);
	vertexBuffer.push_back((float)vertexCoord.z);

	vertexBuffer.push_back((float)textureCoord.x);
	vertexBuffer.push_back((float)textureCoord.y);
}

static float getTextureOriginY(BlockType blockType)
{
	//return glm::vec2(((int)faceType)*TEXTURE_SIDE_LENGTH, (BLOCK_TYPES_TEXTURES_NUMBER-(int)(blockType))*TEXTURE_SIDE_LENGTH);
	return (BLOCK_TYPES_TEXTURES_NUMBER - (int)(blockType)) * TEXTURE_SIDE_LENGTH;
}

static float getTextureCoordX(FaceType faceType)
{
	//return glm::vec2(((int)faceType)*TEXTURE_SIDE_LENGTH, (BLOCK_TYPES_TEXTURES_NUMBER-(int)(blockType))*TEXTURE_SIDE_LENGTH);
	return ((int)faceType) * TEXTURE_SIDE_LENGTH;
}

void Chunk::AddFaceToVertexBuffer(FaceType faceType, glm::vec3 blockCoord, BlockType blockType)
{
	glm::vec3 vertexCoord[6] = {}; // one for each vertex of each face
	glm::vec2 textureCoord[6] = {};

	switch (faceType)
	{
	default:
		break;
	case FaceType::FRONT:
		vertexCoord[0] = glm::vec3(-0.5f, -0.5f, 0.5f); // D
		vertexCoord[1] = glm::vec3(-0.5f, -0.5f, -0.5f); // A
		vertexCoord[2] = glm::vec3(-0.5f, 0.5f, -0.5f); // H

		vertexCoord[3] = glm::vec3(-0.5f, -0.5f, 0.5f); // D
		vertexCoord[4] = glm::vec3(-0.5f, 0.5f, -0.5f); // H
		vertexCoord[5] = glm::vec3(-0.5f, 0.5f, 0.5f); // E


		textureCoord[0] = glm::vec2(1, 0); // D
		textureCoord[1] = glm::vec2(0, 0); // A
		textureCoord[2] = glm::vec2(0, 1); // H

		textureCoord[3] = glm::vec2(1, 0); // D
		textureCoord[4] = glm::vec2(0, 1); // H
		textureCoord[5] = glm::vec2(1, 1); // E
		break;
		
	case FaceType::BACK:
		vertexCoord[0] = glm::vec3(0.5f, -0.5f, -0.5f); // B
		vertexCoord[1] = glm::vec3(0.5f, -0.5f, 0.5f); // C
		vertexCoord[2] = glm::vec3(0.5f, 0.5f, 0.5f); // F

		vertexCoord[3] = glm::vec3(0.5f, -0.5f, -0.5f); // B
		vertexCoord[4] = glm::vec3(0.5f, 0.5f, 0.5f); // F
		vertexCoord[5] = glm::vec3(0.5f, 0.5f, -0.5f); // G


		textureCoord[0] = glm::vec2(1, 0); // B
		textureCoord[1] = glm::vec2(0, 0); // C
		textureCoord[2] = glm::vec2(0, 1); // F

		textureCoord[3] = glm::vec2(1, 0); // B
		textureCoord[4] = glm::vec2(0, 1); // F
		textureCoord[5] = glm::vec2(1, 1); // G
		break;
		
		
	
	case FaceType::LEFT:
		vertexCoord[0] = glm::vec3(-0.5f, -0.5f, -0.5f); // A
		vertexCoord[1] = glm::vec3(0.5f, -0.5f, -0.5f); // B
		vertexCoord[2] = glm::vec3(0.5f, 0.5f, -0.5f); // G

		vertexCoord[3] = glm::vec3(-0.5f, -0.5f, -0.5f); // A
		vertexCoord[4] = glm::vec3(0.5f, 0.5f, -0.5f); // G
		vertexCoord[5] = glm::vec3(-0.5f, 0.5f, -0.5f); // H


		textureCoord[0] = glm::vec2(1, 0); // A
		textureCoord[1] = glm::vec2(0, 0); // B
		textureCoord[2] = glm::vec2(0, 1); // G

		textureCoord[3] = glm::vec2(1, 0); // A
		textureCoord[4] = glm::vec2(0, 1); // G
		textureCoord[5] = glm::vec2(1, 1); // H
		break;
		

	case FaceType::RIGHT:
		vertexCoord[0] = glm::vec3(0.5f, -0.5f, 0.5f); // C
		vertexCoord[1] = glm::vec3(-0.5f, -0.5f, 0.5f); // D
		vertexCoord[2] = glm::vec3(-0.5f, 0.5f, 0.5f); // E

		vertexCoord[3] = glm::vec3(0.5f, -0.5f, 0.5f); // C
		vertexCoord[4] = glm::vec3(-0.5f, 0.5f, 0.5f); // E
		vertexCoord[5] = glm::vec3(0.5f, 0.5f, 0.5f); // F


		textureCoord[0] = glm::vec2(1, 0); // C
		textureCoord[1] = glm::vec2(0, 0); // D
		textureCoord[2] = glm::vec2(0, 1); // E

		textureCoord[3] = glm::vec2(1, 0); // C
		textureCoord[4] = glm::vec2(0, 1); // E
		textureCoord[5] = glm::vec2(1, 1); // F
		break;
		

	case FaceType::UP:
		vertexCoord[0] = glm::vec3(-0.5f, 0.5f, -0.5f); // H
		vertexCoord[1] = glm::vec3(0.5f, 0.5f, -0.5f); // G
		vertexCoord[2] = glm::vec3(0.5f, 0.5f, 0.5f); // F

		vertexCoord[3] = glm::vec3(-0.5f, 0.5f, -0.5f); // H
		vertexCoord[4] = glm::vec3(0.5f, 0.5f, 0.5f); // F
		vertexCoord[5] = glm::vec3(-0.5f, 0.5f, 0.5f); // E


		textureCoord[0] = glm::vec2(0, 0); // H
		textureCoord[1] = glm::vec2(0, 1); // G
		textureCoord[2] = glm::vec2(1, 1); // F

		textureCoord[3] = glm::vec2(0, 0); // H
		textureCoord[4] = glm::vec2(1, 1); // F
		textureCoord[5] = glm::vec2(1, 0); // E
		break;

	case FaceType::BELLOW:
		vertexCoord[0] = glm::vec3(0.5f, -0.5f, -0.5f); // B
		vertexCoord[1] = glm::vec3(-0.5f, -0.5f, -0.5f); // A
		vertexCoord[2] = glm::vec3(-0.5f, -0.5f, 0.5f); // D

		vertexCoord[3] = glm::vec3(0.5f, -0.5f, -0.5f); // B
		vertexCoord[4] = glm::vec3(-0.5f, -0.5f, 0.5f); // D
		vertexCoord[5] = glm::vec3(0.5f, -0.5f, 0.5f); // C


		textureCoord[0] = glm::vec2(0, 0); // B
		textureCoord[1] = glm::vec2(0, 1); // A
		textureCoord[2] = glm::vec2(1, 1); // D

		textureCoord[3] = glm::vec2(0, 0); // B
		textureCoord[4] = glm::vec2(1, 1); // D
		textureCoord[5] = glm::vec2(1, 0); // C
		break;
	}



	float textureOriginX = 0.0f;
	float textureOriginY = 0.0f;
	unsigned int totalHeight = TEXTURE_SIDE_LENGTH * BLOCK_TYPES_TEXTURES_NUMBER;
	unsigned int totalWidth = TEXTURE_SIDE_LENGTH * 6; // 6 = number of face types
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
		textureCoord[i].x = (textureOriginX + textureCoord[i].x * TEXTURE_SIDE_LENGTH) / totalWidth;
		textureCoord[i].y = (textureOriginY + textureCoord[i].y * TEXTURE_SIDE_LENGTH) / totalHeight;

		// Add vertex to the vertex buffer
		addVertexToVertexBuffer(m_vertexBuffer, vertexCoord[i], textureCoord[i]);
	}

}

Chunk::Chunk(ChunkCoord coord)
	: m_coord(coord), m_blocksArray(), m_vertexBuffer()
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

std::vector<float>* Chunk::GetVertexBufferToRender(const bool& chunkChanges, const bool& chunkNorth, const bool& chunkSouth, const bool& chunkWest, const bool& chunkEast)
{
	UpdateInsideVertexBufferToRender();
	UpdateOutsideVertexBufferToRender(chunkNorth, chunkSouth, chunkWest, chunkEast);
	return &m_vertexBuffer;
}

void Chunk::UpdateOutsideVertexBufferToRender(const bool& chunkNorth, const bool& chunkSouth, const bool& chunkWest, const bool& chunkEast)
{
	// TODO: check if there's a block up and down the block if its rendered



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
				if (m_blocksArray[x][y][0]!=BlockType::NONE)
					AddFaceToVertexBuffer(FaceType::BACK, glm::vec3(x,y,0), m_blocksArray[x][y][0]);

			}
		}
	}

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
				if (m_blocksArray[x][y][CHUNK_Z_BLOCK_COUNT - 1] != BlockType::NONE)
					AddFaceToVertexBuffer(FaceType::FRONT, glm::vec3(x, y, CHUNK_Z_BLOCK_COUNT-1), m_blocksArray[x][y][CHUNK_Z_BLOCK_COUNT - 1]);
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
					AddFaceToVertexBuffer(FaceType::LEFT, glm::vec3(0, y, z), m_blocksArray[0][y][z]);
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
					AddFaceToVertexBuffer(FaceType::RIGHT, glm::vec3(CHUNK_X_BLOCK_COUNT-1, y, z), m_blocksArray[CHUNK_X_BLOCK_COUNT - 1][y][z]);
			}
		}
	}
}

void Chunk::UpdateInsideVertexBufferToRender()
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

					/* For each face, check if there's a face next to it ; if not, render the face*/
					// FRONT
					if (m_blocksArray[x][y][z - 1] == BlockType::NONE)
					{
						AddFaceToVertexBuffer(FaceType::FRONT, coord, analysedBlockType);
					}

					// BACK
					if (m_blocksArray[x][y][z + 1] == BlockType::NONE)
					{
						AddFaceToVertexBuffer(FaceType::BACK, coord, analysedBlockType);
					}

					// UP
					if (y==CHUNK_Y_BLOCK_COUNT-1 || m_blocksArray[x][y+1][z] == BlockType::NONE)
					{
						AddFaceToVertexBuffer(FaceType::UP, coord, analysedBlockType);
					}

					// BELLOW
					if (y==0 || m_blocksArray[x][y - 1][z] == BlockType::NONE)
					{
						AddFaceToVertexBuffer(FaceType::BELLOW, coord, analysedBlockType);
					}

					// LEFT
					if (m_blocksArray[x-1][y][z] == BlockType::NONE)
					{
						AddFaceToVertexBuffer(FaceType::LEFT, coord, analysedBlockType);
					}

					// RIGHT
					if (m_blocksArray[x+1][y][z] == BlockType::NONE)
					{
						AddFaceToVertexBuffer(FaceType::RIGHT, coord, analysedBlockType);
					}


					/*AddFaceToVertexBuffer(FaceType::FRONT, coord, analysedBlockType);
					AddFaceToVertexBuffer(FaceType::BACK, coord, analysedBlockType);
					AddFaceToVertexBuffer(FaceType::LEFT, coord, analysedBlockType);
					AddFaceToVertexBuffer(FaceType::RIGHT, coord, analysedBlockType);
					AddFaceToVertexBuffer(FaceType::UP, coord, analysedBlockType);
					AddFaceToVertexBuffer(FaceType::BELLOW, coord, analysedBlockType);*/
				}
			}
		}
	}
}

unsigned int Chunk::GetNumberOfNonAirBlocks(const bool& out) const
{
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
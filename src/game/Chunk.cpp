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

static glm::vec2 getTextureOriginCoord(BlockType blocktype)
{
	return glm::vec2(0, 0);
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
		vertexCoord[0] = glm::vec3(-1, -1, -1); // A
		vertexCoord[1] = glm::vec3(1, -1, -1); // B
		vertexCoord[2] = glm::vec3(1, 1, -1); // G

		vertexCoord[3] = glm::vec3(-1, -1, -1); // A
		vertexCoord[4] = glm::vec3(1, 1, -1); // G
		vertexCoord[5] = glm::vec3(-1, 1, -1); // H


		textureCoord[0]=glm::vec2(0, 0); // A
		textureCoord[1]=glm::vec2(1, 0); // B
		textureCoord[2]=glm::vec2(1, 1); // G

		textureCoord[3]=glm::vec2(0, 0); // A
		textureCoord[4]=glm::vec2(1, 1); // G
		textureCoord[5]=glm::vec2(0, 1); // H
		
	case FaceType::BACK:
		vertexCoord[0] = glm::vec3(1, -1, 1); // C
		vertexCoord[1] = glm::vec3(-1, -1, 1); // D
		vertexCoord[2] = glm::vec3(-1, 1, 1); // E

		vertexCoord[3] = glm::vec3(1, -1, 1); // C
		vertexCoord[4] = glm::vec3(-1, 1, 1); // E
		vertexCoord[5] = glm::vec3(1, 1, 1); // F


		textureCoord[0] = glm::vec2(0, 0); // C
		textureCoord[1] = glm::vec2(1, 0); // D
		textureCoord[2] = glm::vec2(1, 1); // E

		textureCoord[3] = glm::vec2(0, 0); // C
		textureCoord[4] = glm::vec2(1, 1); // E
		textureCoord[5] = glm::vec2(0, 1); // F
	
	case FaceType::LEFT:
		std::cout << "TODO: LEFT FACE SUPPOSED TO BE RENDERED\n";
	case FaceType::RIGHT:
		std::cout << "TODO: RIGHT FACE SUPPOSED TO BE RENDERED\n";
	case FaceType::UP:
		std::cout << "TODO: UP FACE SUPPOSED TO BE RENDERED\n";
	case FaceType::BELLOW:
		std::cout << "TODO: BELLOW FACE SUPPOSED TO BE RENDERED\n";
	}

	// Add the block coord (in world space) to the vertex
	for (int i = 0; i < sizeof(vertexCoord) / sizeof(vertexCoord[0]); i++)
	{
		vertexCoord[i] += blockCoord; // block coord in chunk space
		vertexCoord[i] += glm::vec3(m_coord.idx*CHUNK_X_BLOCK_COUNT, 0, m_coord.idz * CHUNK_Z_BLOCK_COUNT); // chunk coord in world space
	}

	// Add the texture origin (depending on the texture to put on the block)
	glm::vec2 textureOriginCoord = getTextureOriginCoord(blockType);
	for (int i = 0; i < sizeof(textureCoord)/sizeof(textureCoord[0]) ; i++)
	{
		textureCoord[i] += textureOriginCoord;
	}

	// Add it to the vertex buffer
	for (int i = 0; i < sizeof(textureCoord) / sizeof(textureCoord[0]); i++)
	{
		addVertexToVertexBuffer(m_vertexBuffer, vertexCoord[i], textureCoord[i]);
	}
}

Chunk::Chunk(ChunkCoord coord)
	: m_coord(coord), m_blocksArray(), m_vertexBuffer()
{
	// Initialise the chunk to an empty cube of air
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

	m_vertexBuffer.push_back(0.0f);
	m_vertexBuffer.push_back(0.0f);
	//UpdateVertexBufferToRender();
	std::cout << "cunstructor end: " << m_vertexBuffer.size() << "   " << m_vertexBuffer.max_size() << "\n";
}

inline void Chunk::SetBlockType(glm::vec3 block_position, BlockType type)
{
	m_blocksArray[(unsigned int)block_position.x][(unsigned int)block_position.y][(unsigned int)block_position.z] = type;
}

void Chunk::FillPlaneWithBlocks(unsigned int height, BlockType type)
{
	for (unsigned int i = 0; i < CHUNK_X_BLOCK_COUNT ; i++)
	{
		for (unsigned int j = 0 ; j < CHUNK_Z_BLOCK_COUNT; j++)
		{
			m_blocksArray[i][1][j] = type;
		}
	}

}

std::vector<float>* Chunk::GetVertexBufferToRender()
{
	//ClearVertexBuffer();
	UpdateVertexBufferToRender();
	return &m_vertexBuffer;
}

void Chunk::UpdateVertexBufferToRender()
{

	ClearVertexBuffer();
	BlockType analysedBlockType;
	for (unsigned int x = 0; x < CHUNK_X_BLOCK_COUNT; x++)
	{
		for (unsigned int y = 0; y < CHUNK_Y_BLOCK_COUNT; y++)
		{
			for (unsigned int z = 0; z < CHUNK_Z_BLOCK_COUNT; z++)
			{
				analysedBlockType = m_blocksArray[x][y][z];
				if (analysedBlockType != BlockType::NONE && !(x == 0 || y == 0 || z == 0 || x == CHUNK_X_BLOCK_COUNT-1 || y == CHUNK_Y_BLOCK_COUNT-1 || z == CHUNK_Z_BLOCK_COUNT-1)) // else: do not render the face
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
					if (m_blocksArray[x][y+1][z] == BlockType::NONE)
					{
						AddFaceToVertexBuffer(FaceType::UP, coord, analysedBlockType);
					}

					// BELLOW
					if (m_blocksArray[x][y - 1][z] == BlockType::NONE)
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
						AddFaceToVertexBuffer(FaceType::UP, coord, analysedBlockType);
					}

				}
			}
		}
	}
}

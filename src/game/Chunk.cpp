#include "Chunk.hpp"
#include "Constants.hpp"

void Chunk::ClearVertexBuffer()
{
	m_vertexBuffer = {};
}

static void addVertexToVertexBuffer(std::vector<float>& vertexBuffer, glm::vec3 vertexCoord, glm::vec2 textureCoord)
{
	vertexBuffer.push_back(vertexCoord.x);
	vertexBuffer.push_back(vertexCoord.y);
	vertexBuffer.push_back(vertexCoord.z);

	vertexBuffer.push_back(textureCoord.x);
	vertexBuffer.push_back(textureCoord.y);
}

void Chunk::AddFaceToVertexBuffer(FaceType faceType, glm::vec3 blockCoord)
// TODO: Add each vertex to the vertex buffer depending on the faceType and blockCoord (maybe add it at the end), and using addVertexToVertexBuffer
{
	switch (faceType)
	{
	default:
		break;
	case FaceType::FRONT:
		glm::vec2 textureCoord(0, 0);
		
	/*case FaceType::BACK:

	case FaceType::LEFT:

	case FaceType::RIGHT:

	case FaceType::UP:

	case FaceType::BELLOW:*/

	}

	//addVectorsToVertexBuffer(m_vertexBuffer, blockCoord, textureCoord, blockCoord, textureCoord);
}

Chunk::Chunk(ChunkCoord coord)
	: m_coord(coord), m_blocksArray()
{
	for (unsigned int x = 0; x < CHUNK_X_BLOCK_COUNT; x++)
	{
		for (unsigned int y = 0; y <CHUNK_Y_BLOCK_COUNT ; y++)
			m_blocksArray.reserve(CHUNK_Z_BLOCK_COUNT);
	}
}

inline void Chunk::SetBlockType(glm::vec3 block_position, BlockType type)
{
	m_blocksArray[(unsigned int)block_position.x][(unsigned int)block_position.y][(unsigned int)block_position.z] = type;
}

void Chunk::FillPlaneWithBlocks(unsigned int height, BlockType type)
{
	for (unsigned int i = 0; i < 15 ; i++)
	{
		for (unsigned int j = 0 ; j < 15; j++)
		{
			m_blocksArray[i][height][j] = type;
		}
	}
}

std::vector<float> Chunk::GetVertexBufferToRender() const
{
	return m_vertexBuffer;
}

void Chunk::UpdateVertexBufferToRender()
{
	for (unsigned int x = 0; x < CHUNK_X_BLOCK_COUNT; x++)
	{
		for (unsigned int y = 0; y < CHUNK_Y_BLOCK_COUNT; y++)
		{
			for (unsigned int z = 0; z < CHUNK_Z_BLOCK_COUNT; z++)
			{
				if (!(x == 0 || y == 0 || z == 0 || x == CHUNK_X_BLOCK_COUNT-1 || y == CHUNK_Y_BLOCK_COUNT-1 || z == CHUNK_Z_BLOCK_COUNT-1)) // else: do not render the face
				{
					glm::vec3 coord(x, y, z);

					/* For each face, check if there's a face next to it ; if not, render the face*/
					// FRONT
					if (m_blocksArray[x][y][z - 1] == BlockType::NONE)
					{
						AddFaceToVertexBuffer(FaceType::FRONT, coord);
					}

					// BACK
					if (m_blocksArray[x][y][z + 1] == BlockType::NONE)
					{
						AddFaceToVertexBuffer(FaceType::BACK, coord);
					}

					// UP
					if (m_blocksArray[x][y+1][z] == BlockType::NONE)
					{
						AddFaceToVertexBuffer(FaceType::UP, coord);
					}

					// BELLOW
					if (m_blocksArray[x][y - 1][z] == BlockType::NONE)
					{
						AddFaceToVertexBuffer(FaceType::BELLOW, coord);
					}

					// LEFT
					if (m_blocksArray[x-1][y][z] == BlockType::NONE)
					{
						AddFaceToVertexBuffer(FaceType::LEFT, coord);
					}

					// RIGHT
					if (m_blocksArray[x+1][y][z] == BlockType::NONE)
					{
						AddFaceToVertexBuffer(FaceType::UP, coord);
					}

				}
			}
		}
	}
}

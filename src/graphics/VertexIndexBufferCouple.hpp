#pragma once
#include <vector>

struct VertexIndexBufferCouple
{
	std::vector<float> m_vertexBuffer;
	std::vector<unsigned int> m_indexBuffer;

	VertexIndexBufferCouple();
	VertexIndexBufferCouple operator+=(const VertexIndexBufferCouple& couple);
};
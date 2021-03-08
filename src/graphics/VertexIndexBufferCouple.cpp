#include "VertexIndexBufferCouple.hpp"
#include <algorithm>

VertexIndexBufferCouple::VertexIndexBufferCouple()
	:m_vertexBuffer({}), m_indexBuffer({})
{
}

VertexIndexBufferCouple VertexIndexBufferCouple::operator+=(const VertexIndexBufferCouple& couple)
{
	//unsigned int max = *std::max_element(.begin(), vector.end());
	m_vertexBuffer.insert(m_vertexBuffer.end(), (couple.m_vertexBuffer).begin(), (couple.m_vertexBuffer).end());
	m_indexBuffer.insert(m_indexBuffer.end(), (couple.m_indexBuffer).begin(), (couple.m_indexBuffer).end());
	return *this;
}

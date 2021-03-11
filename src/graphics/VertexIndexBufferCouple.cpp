#include "VertexIndexBufferCouple.hpp"
#include <algorithm>

VertexIndexBufferCouple::VertexIndexBufferCouple()
	:m_vertexBuffer({}), m_indexBuffer({}), m_indexCount(0)
{
}

VertexIndexBufferCouple VertexIndexBufferCouple::operator+=(const VertexIndexBufferCouple& couple)
{
	m_vertexBuffer.insert(m_vertexBuffer.end(), (couple.m_vertexBuffer).begin(), (couple.m_vertexBuffer).end());
	m_indexBuffer.insert(m_indexBuffer.end(), (couple.m_indexBuffer).begin(), (couple.m_indexBuffer).end());

	m_indexCount = couple.m_indexCount;
	return *this;
}

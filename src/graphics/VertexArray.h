#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
private:
	unsigned int m_RendererID;
	VertexBufferLayout m_bufferLayout;

public:
	VertexArray(VertexBufferLayout bufferLayout);
	~VertexArray();

	void Bind() const;
	void Unbind() const;
	void DeleteVertexArray() const;

	//void AddBuffer(const VertexBufferLayout& layout);
	void AddBuffer(const VertexBuffer& vb);
};
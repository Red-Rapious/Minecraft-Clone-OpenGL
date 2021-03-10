#pragma once

class VertexBuffer
{
private :
	unsigned int m_RendererID;
public :
	VertexBuffer();
	void DeleteBuffer();
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
	void Init(const void* data, unsigned int size, const bool& sub = false);
};
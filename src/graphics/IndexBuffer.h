#pragma once

class IndexBuffer
{
private :
	unsigned int m_RendererID;
	unsigned int m_Count;
public :
	IndexBuffer();
	~IndexBuffer();

	void DeleteBuffer();
	void Bind() const;
	void Unbind() const;
	void Init(const unsigned int* data, unsigned int count, const bool& sub = false);

	inline unsigned int GetCount() const { return m_Count; }

};
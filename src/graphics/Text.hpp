#pragma once
#include <string>

#include <glad/glad.h>
#include "Texture.h"
#include "VertexArray.h"

class Text
{
private:
	Texture m_texture;
	unsigned int m_vertexBufferID;

	void GenerateBuffer();

public:
	Text(const std::string path);
	inline ~Text() { CleanupText(); };
	void PrintText(const VertexArray &vao, const std::string text, const int& x, const int& y, const unsigned int& size);
	void CleanupText();
};
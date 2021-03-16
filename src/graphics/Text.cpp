#include "Text.hpp"

#include "stb/stb_image.h"

Text::Text(const std::string path)
{
	stbi_set_flip_vertically_on_load(1);
	m_LocalBuffer = stbi_load(path.c_str(), 0, 0, 0, 4);

	// Initialize vertex buffer
	glGenBuffers(1, &m_vbID);
	glGenBuffers(1, &m_uvID);
}

void Text::PrintText(const char* text, const int& x, const int& y, const unsigned int& size)
{
}

void Text::CleanupText()
{
}
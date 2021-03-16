#pragma once
#include <string>

#include <glad/glad.h>

class Text
{
private:
	unsigned char* m_LocalBuffer;
	unsigned int m_vbID;
	unsigned int m_uvID;
public:
	Text(const std::string path);
	inline ~Text() { CleanupText(); };
	void PrintText(const char* text, const int& x, const int& y, const unsigned int& size);
	void CleanupText();
};
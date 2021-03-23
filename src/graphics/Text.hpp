#pragma once
#include <string>

#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include "Texture.h"
#include "VertexArray.h"
#include "Shader.h"

class Text
{
private:
	Texture m_texture;
	unsigned int m_vertexBufferID;

	void GenerateBuffer();

public:
	Text(const std::string path);
	void PrintText(GLFWwindow* window, const VertexArray &vao, Shader* shader, const std::string text, const int& x, const int& y, const unsigned int& size);
};
#pragma once

#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include "Texture.h"
#include "VertexArray.h"
#include "Shader.h"

class Crossair
{
private:
	Texture m_texture;
	unsigned int m_vertexBufferID;

	void GenerateBuffer();

public:
	Crossair(const std::string path);
	void RenderCrossair(GLFWwindow* window, const VertexArray& vao, Shader* shader, const unsigned int size);

};
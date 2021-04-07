#pragma once

#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include "Texture.h"
#include "VertexArray.h"
#include "Shader.h"

class Image
{
private:
	Texture m_texture;
	unsigned int m_vertexBufferID;

	void GenerateBuffer();

public:
	Image(const std::string path);
	void RenderImage(GLFWwindow* window, const VertexArray& vao, Shader* shader, const unsigned int size);

};
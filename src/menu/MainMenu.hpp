#pragma once

#include "../graphics/Renderer.h"
#include "../graphics/IndexBuffer.h"
#include "../graphics/VertexBuffer.h"
#include "../graphics/VertexBufferLayout.h"
#include "../graphics/Shader.h"
#include "../graphics/Texture.h"
#include "../graphics/Controls.h"
#include "../graphics/Text.hpp"
#include "../graphics/Image.hpp"


class MainMenu
{
private:
	VertexArray m_vao;
	Shader m_basicShader;
	Image m_logoImage;
	Text m_text;
	Renderer m_renderer;

public:
	MainMenu(GLFWwindow* window);
	bool UpdateFrame(GLFWwindow* window);
};
#pragma once

#include "../graphics/Renderer.h"
#include "../graphics/IndexBuffer.h"
#include "../graphics/VertexBuffer.h"
#include "../graphics/VertexBufferLayout.h"
#include "../graphics/Shader.h"
#include "../graphics/Texture.h"
#include "../graphics/Controls.h"
#include "../graphics/Text.hpp"
#include "../graphics/Crossair.hpp"

#include "Map.hpp"

class World
{
private:
	Control m_control;
	Map m_map;
	VertexArray m_vao;
	Shader m_blocksShader;
	Shader m_basicShader;
	Texture m_blocksTexture;
	Text m_text;
	Crossair m_crossair;
	Renderer m_renderer;

	const int m_textSize = 18;
	const char* m_openGLVersion = (char*)glGetString(GL_VERSION);
	bool m_displayDebugText = true;
	const unsigned int m_defaultSwitchKeyCounter = 10;
	int m_switchKeyCounter = 0; // number of frames to wait before switching the mode again
	double m_lastTime = glfwGetTime();
	
public:
	World(GLFWwindow* window, glm::vec3 defaultCameraPosition);
	void UpdateFrame(GLFWwindow* window);
};
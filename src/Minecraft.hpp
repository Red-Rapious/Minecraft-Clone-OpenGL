#include "graphics/Controls.h"
#include "game/Map.hpp"
#include "graphics/VertexArray.h"
#include "graphics/Shader.h"
#include "graphics/Texture.h"
#include "graphics/Renderer.h"

class Minecraft
{
private:
	Control m_control;
	Map m_map;
	VertexArray m_vertexArray;
	Shader m_blocksShader;
	Texture m_blocksTexture;
	Renderer m_renderer;

public:
	Minecraft(GLFWwindow* window);
	void Update(GLFWwindow* window);
};
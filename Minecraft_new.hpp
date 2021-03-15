#include "graphics/Controls.h"
#include "game/Map.hpp"
#include "graphics/VertexArray.h"
#include "graphics/Shader.h"
#include "graphics/Texture.h"
#include "graphics/Renderer.h"

#include <memory>

class Minecraft
{
private:
	std::unique_ptr<Control> m_control;
	std::unique_ptr<Map> m_map;
	std::unique_ptr<VertexArray> m_vertexArray;
	std::unique_ptr<Shader> m_blocksShader;
	std::unique_ptr<Texture> m_blocksTexture;
	std::unique_ptr<Renderer> m_renderer;

public:
	Minecraft(GLFWwindow* window);
	void Update(GLFWwindow* window);
};
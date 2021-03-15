#include "Minecraft.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

Minecraft::Minecraft(GLFWwindow* window)
{
    m_map = std::unique_ptr<Map>(new Map);

	glm::vec3 camera_position(8, 15, 8);
	m_control = std::unique_ptr<Control>(new Control(window, camera_position));

    m_vertexArray = std::unique_ptr<VertexArray>(new VertexArray);
	m_vertexArray->Bind();

	
    m_blocksShader = std::unique_ptr<Shader>(new Shader("res/shaders/Basic.shader"));
	m_blocksShader->Bind();
	m_blocksShader->SetUniform1i("u_Texture", 0); // 0 = slot, default value

	
    m_blocksTexture = std::unique_ptr<Texture>(new Texture("res/textures/default_mc_textures.png"));
    m_blocksTexture->Bind();

    m_renderer = std::unique_ptr<Renderer>(new Renderer);
}

void Minecraft::Update(GLFWwindow* window)
{
    m_renderer->Clear();

    m_control->UpdateInput(window);
    m_map->UpdateChunkPlayerPosition(m_control->GetCameraPosition());

    m_map->GenerateOneChunk();

    glm::mat4 proj = m_control->getProjectionMatrix();
    glm::mat4 view = m_control->getViewMatrix();
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

    glm::mat4 MVP = proj * view * model;
    m_blocksShader->SetUniformMat4f("u_MVP", MVP);

    m_map->RenderAllNeededChunks(*m_vertexArray);

    /* Swap front and back buffers */
    glfwSwapBuffers(window);
    /* Poll for and process events */
    glfwPollEvents();
}

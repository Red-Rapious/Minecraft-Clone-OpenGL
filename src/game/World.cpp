#include "World.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

World::World(GLFWwindow* window, glm::vec3 defaultCameraPosition)
	: m_control(window, defaultCameraPosition), m_blocksShader("res/shaders/Blocks.shader"), m_textShader("res/shaders/Text.shader"), m_blocksTexture("res/textures/default_mc_textures.png"), m_text("res/textures/ascii.png"), m_crossair("res/textures/crossair.png")
{
	m_vao.Bind();

	m_blocksShader.Bind();
	m_blocksShader.SetUniform1i("u_Texture", 0);

	m_textShader.Bind();
	m_textShader.SetUniform1i("u_Texture", 0);

	m_blocksTexture.Bind();
}

void World::UpdateFrame(GLFWwindow* window)
{
    m_renderer.Clear();

    m_control.UpdateInput();
    glm::vec3 cameraPosition = m_control.GetCameraPosition();
    m_map.UpdateChunkPlayerPosition(cameraPosition);

    m_map.GenerateOneChunk(); // Generate only one chunk per frame to avoid huge freezes

    // Generate the matrices
    glm::mat4 proj = m_control.getProjectionMatrix();
    glm::mat4 view = m_control.getViewMatrix();
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

    glm::mat4 MVP = proj * view * model;
    m_blocksShader.Bind();
    m_blocksShader.SetUniformMat4f("u_MVP", MVP);

    m_blocksTexture.Bind();
    m_map.RenderAllNeededChunks(m_vao);



    if (m_displayDebugText)
    {
        m_textShader.Bind();
        m_textShader.SetUniform4f("u_Color", 1.0, 1.0, 1.0, 1.0); // set text color
        {
            // Camera position
            std::string x = std::to_string((int)cameraPosition.x);
            std::string y = std::to_string((int)cameraPosition.y);
            std::string z = std::to_string((int)cameraPosition.z);
            m_text.PrintText(window, m_vao, &m_textShader, "Camera position : x = " + x + " y = " + y + " z=" + z, 10, 10, m_textSize);
        }

        // Player in chunk position
        ChunkCoord chunkCoord = m_map.GetPlayerPosition();
        m_text.PrintText(window, m_vao, &m_textShader, "Actual chunk: x = " + std::to_string(chunkCoord.idx) + " z=" + std::to_string(chunkCoord.idz), 10, 10 + m_textSize, m_textSize);

        // OpenGL version
        m_text.PrintText(window, m_vao, &m_textShader, m_openGLVersion, m_textSize / 2, 745, m_textSize);

        // SPF and FPS counters
        float spf = glfwGetTime() - m_lastTime;
        float fps = 1 / spf;
        if (fps < 30)
        {
            m_textShader.SetUniform4f("u_Color", 1.0, 0.2, 0.20, 1.0);
        }
        else if (fps > 60)
        {
            m_textShader.SetUniform4f("u_Color", 0.2, 1.0, 0.33, 1.0);
        }
        m_text.PrintText(window, m_vao, &m_textShader, "Delta = " + std::to_string((int)(spf * 1000)) + "ms   FPS=" + std::to_string((int)fps), 10, 10 + 2 * m_textSize, m_textSize);

    }
    m_lastTime = glfwGetTime();

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        if (m_switchKeyCounter == 0)
        {
            m_displayDebugText = !m_displayDebugText;
            m_switchKeyCounter = m_defaultSwitchKeyCounter;
        }
    }
    if (m_switchKeyCounter > 0)
        m_switchKeyCounter--;


    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) // change to click later
    {
        glm::vec3 block = m_control.GetAimedBlock(m_map.GetChunkByCoord(m_map.ConvertPositionToChunkCoord(cameraPosition))->GetBlocksArray());
        std::cout << block.x << "  " << block.y << "  " << block.z << "\n";
    }


    m_textShader.SetUniform4f("u_Color", 1.0, 1.0, 1.0, 1.0);
    m_crossair.RenderCrossair(window, m_vao, &m_textShader, 20); // always keep an even number here
}
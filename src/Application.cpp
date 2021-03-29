#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <unordered_map>
#include <string>

#include "graphics/Renderer.h"
#include "graphics/IndexBuffer.h"
#include "graphics/VertexBuffer.h"
#include "graphics/VertexArray.h"
#include "graphics/VertexBufferLayout.h"
#include "graphics/Shader.h"
#include "graphics/Texture.h"
#include "graphics/Controls.h"
#include "graphics/VertexIndexBufferCouple.hpp"
#include "graphics/Text.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

#include "stb/stb_image.h"

#include "game/Chunk.hpp"
#include "game/Map.hpp"

constexpr auto W_WIDTH = 1024;
constexpr auto W_HEIGHT = 768;
constexpr auto FULLSCREEN = false;

int main(void)
{
    GLFWwindow* window;

    /* Initialize GLFW */
    if (!glfwInit())
    {
        std::cout << "[Initialisation error] Unable to initialise GLFW" << std::endl;
        ASSERT(false);
        return -1;
    }
        
    /* Setting profile to CORE to use vertex arrays objects */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    if (FULLSCREEN)
        window = glfwCreateWindow(1920, 1280, "Minecraft", glfwGetPrimaryMonitor(), NULL);
    else
        window = glfwCreateWindow(W_WIDTH, W_HEIGHT, "Minecraft", NULL, NULL); // 4:3
    glfwSetWindowPos(window, 500, 200);

    if (!window)
    {
        std::cout << "[Initialisation error] Unable to create a window" << std::endl;
        glfwTerminate();
        ASSERT(false);
        return -1;
    }

    GLFWimage icons[1];
    icons[0].pixels = stbi_load("res/icons/icon512.png", &icons[0].width, &icons[0].height, 0, 4);

    glfwSetWindowIcon(window, 1, icons);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Set the swap interval to make the render smooth */
    glfwSwapInterval(1);

    /* Initialising GLEW */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "[Initialisation error] Unable to initialise GLEW" << std::endl;
        ASSERT(false);
        return -1;
    }

    {
        /* GL enables */
        // Enable depth test
        GLCall(glEnable(GL_DEPTH_TEST));
        // Accept fragment if it closer to the camera than the former one
        GLCall(glDepthFunc(GL_LESS));
        
        // Cull face if its BEHIND - GL_FRONT is here to fix a strange bug
        GLCall(glEnable(GL_CULL_FACE));
        GLCall(glCullFace(GL_FRONT));

        // Enable blend for transparency
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        
        // Change clear color to a blue sky
        GLCall(glClearColor(0.53, 0.81, 0.92, 1.0));

        srand(time(0)); // Init seed for the trees creation

        /* Non-rendering bjects instanciation */
        glm::vec3 camera_position(8, 50, 8);

        Control control(window, camera_position);
        Map map;

        /* Graphics part */
        VertexArray vao;
        vao.Bind();

        Shader blocksShader("res/shaders/Blocks.shader");
        blocksShader.Bind();
        blocksShader.SetUniform1i("u_Texture", 0); // 0 = slot, default value

        Shader textShader("res/shaders/Text.shader");
        textShader.Bind();
        textShader.SetUniform1i("u_Texture", 0); // 0 = slot, default value

        Texture blocksTexture("res/textures/default_mc_textures.png");
        blocksTexture.Bind(); // default slot is 0

        Renderer renderer;
        double lastTime = glfwGetTime();

        Text text("res/textures/ascii.png");
        const int textSize = 18;
        const char* openGLVersion = (char*)glGetString(GL_VERSION);
        bool displayDebugText = true;
        const unsigned int defaultSwitchKeyCounter = 10;
        int switchKeyCounter = 0; // number of frames to wait before switching the mode again

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
        {
            renderer.Clear();

            control.UpdateInput();
            camera_position = control.GetCameraPosition();
            map.UpdateChunkPlayerPosition(camera_position);

            map.GenerateOneChunk(); // Generate only one chunk per frame to avoid huge freezes

            // Generate the matrices
            glm::mat4 proj = control.getProjectionMatrix();
            glm::mat4 view = control.getViewMatrix();
            glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

            glm::mat4 MVP = proj * view * model;
            blocksShader.Bind();
            blocksShader.SetUniformMat4f("u_MVP", MVP);

            blocksTexture.Bind();
            map.RenderAllNeededChunks(vao);

            if (displayDebugText)
            {
                textShader.Bind();
                textShader.SetUniform4f("u_Color", 1.0, 1.0, 1.0, 1.0); // set text color
                {
                    // Camera position
                    std::string x = std::to_string((int)camera_position.x);
                    std::string y = std::to_string((int)camera_position.y);
                    std::string z = std::to_string((int)camera_position.z);
                    text.PrintText(window, vao, &textShader, "Camera position : x = " + x + " y = " + y + " z=" + z, 10, 10, textSize);
                }

                // Player in chunk position
                ChunkCoord chunkCoord = map.GetPlayerPosition();
                text.PrintText(window, vao, &textShader, "Actual chunk: x = " + std::to_string(chunkCoord.idx) + " z=" + std::to_string(chunkCoord.idz), 10, 10 + textSize, textSize);

                // OpenGL version
                text.PrintText(window, vao, &textShader, openGLVersion, 10, 745, 18);

                // SPF and FPS counters
                float spf = glfwGetTime() - lastTime;
                float fps = 1 / spf;
                if (fps < 30)
                {
                    textShader.SetUniform4f("u_Color", 1.0, 0.2, 0.20, 1.0);
                }
                else if (fps > 60)
                {
                    textShader.SetUniform4f("u_Color", 0.2, 1.0, 0.33, 1.0);
                }
                text.PrintText(window, vao, &textShader, "Delta = " + std::to_string((int)(spf * 1000)) + "ms   FPS=" + std::to_string((int)fps), 10, 10 + 2 * textSize, textSize);
                
            }
            lastTime = glfwGetTime();

            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            {
                if (switchKeyCounter == 0)
                {
                    displayDebugText = !displayDebugText;
                    switchKeyCounter = defaultSwitchKeyCounter;
                }     
            }
            if (switchKeyCounter > 0)
                switchKeyCounter--;

            /* Swap front and back buffers */
            glfwSwapBuffers(window);
            /* Poll for and process events */
            glfwPollEvents();

        } // end of main loop
    } // end of OpenGL scope
    glfwTerminate();

    return 0;
}
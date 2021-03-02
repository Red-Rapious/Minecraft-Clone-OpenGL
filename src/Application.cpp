#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Renderer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

#define W_WIDTH 1024
#define W_HEIGHT 768

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
    window = glfwCreateWindow(W_WIDTH, W_HEIGHT, "Hello World", NULL, NULL); // 4:3

    if (!window)
    {
        std::cout << "[Initialisation error] Unable to create a window" << std::endl;
        glfwTerminate();
        ASSERT(false);
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Set the swap interval to make the render smooth */
    glfwSwapInterval(1);

    /* Initialising GLEW */
    if (glewInit() != GLEW_OK)
    {
        std::cout << "[Initialisation error] Unable to initialise GLEW" << std::endl;
        ASSERT(false);
        return -1;
    }

    {
        float positions[] = {
            -1.0f, -1.0f,  0.0f,
             1.0f, -1.0f,  0.0f,
             0.0f,  1.0f,  0.0f
        };

        glm::vec3 camera_position(4, 3, 3);

        VertexArray va;
        VertexBuffer vb(positions, 3 * 3 * sizeof(float)); // number of vertices stored * floats per vertex
        VertexBufferLayout layout;

        layout.Push<float>(3); // set the layout to a couple of floats
        va.AddBuffer(vb, layout); // give the layout to opengl

        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 1.0f, 0.0f, 0.0f, 0.0f);

        
        /*glm::mat4 proj = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0), glm::vec3(0,0,0));
        glm::mat4 model = glm::translate(glm::mat4(1.0), glm::vec3(0,0,0));*/

        glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)W_WIDTH / (float)W_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(camera_position, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)) * glm::mat4(1.0f);

        float z = 0.0f;

        Renderer renderer;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            renderer.Clear();

            z += 0.01f;
            glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, z));
            glm::mat4 MVP = proj * view * model;
            shader.SetUniformMat4f("u_MVP", MVP);


            renderer.Draw(va, 3, shader);


            /* Swap front and back buffers */
            glfwSwapBuffers(window);
            /* Poll for and process events */
            glfwPollEvents();
        }

    }
    glfwTerminate();

    return 0;
}
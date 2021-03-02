#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Renderer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

#define W_WIDTH 1024
#define W_HEIGHT 768

static const GLfloat cube_triangles_positions[] = {
    -1.0f,-1.0f,-1.0f,    1.0f,  0.0f, // A
    -1.0f,-1.0f, 1.0f,    0.0f,  0.0f, // D
    -1.0f, 1.0f, 1.0f,    0.0f,  1.0f, // E
    
     1.0f, 1.0f,-1.0f,    1.0f,  1.0f, // G
    -1.0f,-1.0f,-1.0f,    0.0f,  0.0f, // A
    -1.0f, 1.0f,-1.0f,    0.0f,  1.0f, // H

     1.0f,-1.0f, 1.0f,    0.0f,  1.0f, // C
    -1.0f,-1.0f,-1.0f,    1.0f,  0.0f, // A
     1.0f,-1.0f,-1.0f,    0.0f,  0.0f, // B

     1.0f, 1.0f,-1.0f,    1.0f,  1.0f, // G
     1.0f,-1.0f,-1.0f,    1.0f,  0.0f, // B
    -1.0f,-1.0f,-1.0f,    0.0f,  0.0f, // A

    -1.0f,-1.0f,-1.0f,    1.0f,  0.0f, // A
    -1.0f, 1.0f, 1.0f,    0.0f,  1.0f, // E
    -1.0f, 1.0f,-1.0f,    1.0f,  1.0f, // H

     1.0f,-1.0f, 1.0f,    0.0f,  1.0f, // C
    -1.0f,-1.0f, 1.0f,    1.0f,  1.0f, // D
    -1.0f,-1.0f,-1.0f,    1.0f,  0.0f, // A

    -1.0f, 1.0f, 1.0f,    1.0f,  1.0f, // E
    -1.0f,-1.0f, 1.0f,    1.0f,  0.0f, // D
     1.0f,-1.0f, 1.0f,    0.0f,  0.0f, // C

     1.0f, 1.0f, 1.0f,    1.0f,  1.0f, // F
     1.0f,-1.0f,-1.0f,    0.0f,  0.0f, // B
     1.0f, 1.0f,-1.0f,    0.0f,  1.0f, // G

     1.0f,-1.0f,-1.0f,    0.0f,  0.0f, // B
     1.0f, 1.0f, 1.0f,    1.0f,  1.0f, // F
     1.0f,-1.0f, 1.0f,    1.0f,  0.0f, // C

     1.0f, 1.0f, 1.0f,    1.0f,  1.0f, // F
     1.0f, 1.0f,-1.0f,    1.0f,  0.0f, // G
    -1.0f, 1.0f,-1.0f,    0.0f,  0.0f, // H

     1.0f, 1.0f, 1.0f,    1.0f,  1.0f, // F
    -1.0f, 1.0f,-1.0f,    0.0f,  0.0f, // H
    -1.0f, 1.0f, 1.0f,    0.0f,  1.0f, // E

     1.0f, 1.0f, 1.0f,    0.0f,  1.0f, // F
    -1.0f, 1.0f, 1.0f,    1.0f,  1.0f, // E
     1.0f,-1.0f, 1.0f,    1.0f,  0.0f  // C
};

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
    window = glfwCreateWindow(W_WIDTH, W_HEIGHT, "OpenGL", NULL, NULL); // 4:3

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

    // Enable depth test
    GLCall(glEnable(GL_DEPTH_TEST));
    // Accept fragment if it closer to the camera than the former one
    GLCall(glDepthFunc(GL_LESS));

    /* Initialising GLEW */
    if (glewInit() != GLEW_OK)
    {
        std::cout << "[Initialisation error] Unable to initialise GLEW" << std::endl;
        ASSERT(false);
        return -1;
    }

    {

        glm::vec3 camera_position(4, 3, 3);

        VertexArray va;
        VertexBuffer vb(cube_triangles_positions, 12* 3 * 3 * sizeof(float)); // number of vertices stored * floats per vertex
        VertexBufferLayout layout;

        layout.Push<float>(3); // add 3 floats for the vertex positions
        layout.Push<float>(2); // add 2 floats for the texture coords
        va.AddBuffer(vb, layout); // give the layout to opengl

        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
        //shader.SetUniform4f("u_Color", 1.0f, 0.0f, 0.0f, 0.0f);


        glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)W_WIDTH / (float)W_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(camera_position, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
        glm::mat4 MVP = proj * view * model;
        shader.SetUniformMat4f("u_MVP", MVP);

        Texture texture("res/textures/image.png");
        texture.Bind(); // default slot is 0
        shader.SetUniform1i("u_Texture", 0); // 0 = slot, default value

        Renderer renderer;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            renderer.Clear();

            renderer.Draw(va, 12*3, shader);


            /* Swap front and back buffers */
            glfwSwapBuffers(window);
            /* Poll for and process events */
            glfwPollEvents();
        }

    }
    glfwTerminate();

    return 0;
}
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <unordered_map>

#include "graphics/Renderer.h"
#include "graphics/IndexBuffer.h"
#include "graphics/VertexBuffer.h"
#include "graphics/VertexArray.h"
#include "graphics/VertexBufferLayout.h"
#include "graphics/Shader.h"
#include "graphics/Texture.h"
#include "graphics/Controls.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

#include "game/Chunck.h"

constexpr auto W_WIDTH = 1024;
constexpr auto W_HEIGHT = 768;


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
     1.0f,-1.0f, 1.0f,    0.0f,  0.0f  // C
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
    //glfwSetWindowPos(window, 0, 0);

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
        // Enable depth test
        GLCall(glEnable(GL_DEPTH_TEST));
        // Accept fragment if it closer to the camera than the former one
        GLCall(glDepthFunc(GL_LESS));
        GLCall(glEnable(GL_CULL_FACE));
        GLCall(glClearColor(0.0, 0.0, 0.0, 1.0));

        glm::vec3 camera_position(3, 3, 3);

        VertexArray va;
        VertexBuffer vb(cube_triangles_positions, 12* (3+2) * 3 * sizeof(float)); // number of vertices stored * floats per vertex
        VertexBufferLayout layout;

        layout.Push<float>(3); // add 3 floats for the vertex positions
        layout.Push<float>(2); // add 2 floats for the texture coords
        va.AddBuffer(vb, layout); // give the layout to opengl

        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
        

        Texture texture("res/textures/image.png");
        texture.Bind(); // default slot is 0
        shader.SetUniform1i("u_Texture", 0); // 0 = slot, default value

        Renderer renderer;
        Control control(window, camera_position);


        std::unordered_map<ChunckCoord, Chunck*, ChunkCoordHash> chuncksCoordsMap;
        ChunckCoord coord(0, 0);
        Chunck chunck(coord);
        chuncksCoordsMap[chunck.GetCoord()] = &chunck;

        chunck.FillPlaneWithBlocks(0, BlockType::GRASS);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
        {
            renderer.Clear();

            control.UpdateInput();

            glm::mat4 proj = control.getProjectionMatrix();
            glm::mat4 view = control.getViewMatrix();
            glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

            glm::mat4 MVP = proj * view * model;
            shader.SetUniformMat4f("u_MVP", MVP);

            renderer.Draw(va, 12*(3+2), shader);

            /* Swap front and back buffers */
            glfwSwapBuffers(window);
            /* Poll for and process events */
            glfwPollEvents();
        }

    }
    glfwTerminate();

    return 0;
}
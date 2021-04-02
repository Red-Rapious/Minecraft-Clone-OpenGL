#include "Crossair.hpp"

Crossair::Crossair(const std::string path)
    :m_texture(path), m_vertexBufferID(-1)
{
    GenerateBuffer();
}

void Crossair::GenerateBuffer()
{
    GLCall(glGenBuffers(1, &m_vertexBufferID));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexBufferID));
}

void Crossair::RenderCrossair(GLFWwindow* window, const VertexArray& vao, Shader* shader, const unsigned int size)
{
    int windowX;
    int windowY;
    GLCall(glfwGetWindowSize(window, &windowX, &windowY));

    shader->Bind();
    shader->SetUniform1i("u_windowSizeX", windowX);
    shader->SetUniform1i("u_windowSizeY", windowY);

    float vertexBuffer[24] = 
    {
        windowX / 2 - size / 2, windowY / 2 - size / 2, 0, 0, // dl
        windowX / 2 - size / 2, windowY / 2 + size / 2, 0, 1, // ul
        windowX / 2 + size / 2, windowY / 2 - size / 2, 1, 0, // dr

        windowX / 2 - size / 2, windowY / 2 + size / 2, 0, 1, // ul
        windowX / 2 + size / 2, windowY / 2 + size / 2, 1, 1, // ur
        windowX / 2 + size / 2, windowY / 2 - size / 2, 1, 0, // dr
    };
    
    //const unsigned int charWidth = size * 0.8;

    //for (unsigned int i = 0; i < text.size(); i++) // for each character of the string
    //{
    //    char character = text[i];
    //    float uv_x = (character % 16) / 16.0f;
    //    float uv_y = (character / 16) / 16.0f;

    //    vertexBuffer.push_back(x + i * charWidth); //ul
    //    vertexBuffer.push_back(y + size);
    //    vertexBuffer.push_back(uv_x);
    //    vertexBuffer.push_back(1.0f - uv_y);

    //    vertexBuffer.push_back(x + i * charWidth + charWidth); //ur
    //    vertexBuffer.push_back(y + size);
    //    vertexBuffer.push_back(uv_x + 1.0f / 16.0f);
    //    vertexBuffer.push_back(1.0f - uv_y);

    //    vertexBuffer.push_back(x + i * charWidth); //dl
    //    vertexBuffer.push_back(y);
    //    vertexBuffer.push_back(uv_x);
    //    vertexBuffer.push_back(1.0f - (uv_y + 1.0f / 16.0f));



    //    vertexBuffer.push_back(x + i * charWidth + charWidth); //dr
    //    vertexBuffer.push_back(y);
    //    vertexBuffer.push_back(uv_x + 1.0f / 16.0f);
    //    vertexBuffer.push_back(1.0f - (uv_y + 1.0f / 16.0f));

    //    vertexBuffer.push_back(x + i * charWidth); //dl
    //    vertexBuffer.push_back(y);
    //    vertexBuffer.push_back(uv_x);
    //    vertexBuffer.push_back(1.0f - (uv_y + 1.0f / 16.0f));

    //    vertexBuffer.push_back(x + i * charWidth + charWidth); //ur
    //    vertexBuffer.push_back(y + size);
    //    vertexBuffer.push_back(uv_x + 1.0f / 16.0f);
    //    vertexBuffer.push_back(1.0f - uv_y);
    //}

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferID));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * 24, vertexBuffer, GL_STATIC_DRAW));

    VertexBufferLayout bufferLayout;

    bufferLayout.Push<float>(2); // add 2 floats for the vertex positions
    bufferLayout.Push<float>(2); // add 2 floats for the texture coords
    vao.AddBuffer(bufferLayout);

    m_texture.Bind();
    GLCall(glDrawArrays(GL_TRIANGLES, 0, 24));
}
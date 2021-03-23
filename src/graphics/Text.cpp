#include "Text.hpp"
#include "glm/glm.hpp"

#include <vector>
#include "Utility.hpp"

Text::Text(const std::string path)
	:m_texture(path), m_vertexBufferID(-1)
{
    GenerateBuffer();
}

void Text::GenerateBuffer()
{
    GLCall(glGenBuffers(1, &m_vertexBufferID));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexBufferID));
}

void Text::PrintText(const VertexArray& vao, const std::string text, const int& x, const int& y, const unsigned int& size)
{
    //std::vector<glm::vec2> vertices;
    //std::vector<glm::vec2> UVs;
    std::vector<float> vertexBuffer;

	for (unsigned int i = 0; i < text.size(); i++) // for each character of the string
	{
        char character = text[i];
        float uv_x = (character % 16) / 16.0f;
        float uv_y = (character / 16) / 16.0f;

        vertexBuffer.push_back(x + i * size); //ul
        vertexBuffer.push_back(y + size);
        vertexBuffer.push_back(uv_x);
        vertexBuffer.push_back(1.0f - uv_y);

        vertexBuffer.push_back(x + i * size + size); //ur
        vertexBuffer.push_back(y + size);
        vertexBuffer.push_back(uv_x + 1.0f / 16.0f);
        vertexBuffer.push_back(1.0f - uv_y);

        vertexBuffer.push_back(x + i * size); //dl
        vertexBuffer.push_back(y);
        vertexBuffer.push_back(uv_x);
        vertexBuffer.push_back(1.0f - (uv_y + 1.0f / 16.0f));



        vertexBuffer.push_back(x + i * size + size); //dr
        vertexBuffer.push_back(y);
        vertexBuffer.push_back(uv_x + 1.0f / 16.0f);
        vertexBuffer.push_back(1.0f - (uv_y + 1.0f / 16.0f));

        vertexBuffer.push_back(x + i * size); //dl
        vertexBuffer.push_back(y);
        vertexBuffer.push_back(uv_x);
        vertexBuffer.push_back(1.0f - (uv_y + 1.0f / 16.0f));

        vertexBuffer.push_back(x + i * size + size); //ur
        vertexBuffer.push_back(y + size);
        vertexBuffer.push_back(uv_x + 1.0f / 16.0f);
        vertexBuffer.push_back(1.0f - uv_y);
	}

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferID));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * vertexBuffer.size(), vertexBuffer.data(), GL_STATIC_DRAW));

    VertexBufferLayout bufferLayout;

    bufferLayout.Push<float>(2); // add 2 floats for the vertex positions
    bufferLayout.Push<float>(2); // add 2 floats for the texture coords
    vao.AddBuffer(bufferLayout);

    m_texture.Bind();
    GLCall(glDrawArrays(GL_TRIANGLES, 0, vertexBuffer.size()));

}

void Text::CleanupText()
{
}
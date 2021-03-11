#include "IndexBuffer.h"
#include "Renderer.h"
#include "Utility.hpp"

IndexBuffer::IndexBuffer()
    : m_Count(0), m_RendererID(0)
{ 
    GLCall(glGenBuffers(1, &m_RendererID));
}

IndexBuffer::~IndexBuffer()
{
    DeleteBuffer();
}

void IndexBuffer::DeleteBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void IndexBuffer::Init(const unsigned int* data, unsigned int count, const bool& sub)
{
    ASSERT(sizeof(unsigned int) == sizeof(GLuint));

    m_Count = count;
    
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}
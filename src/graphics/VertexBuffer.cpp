#include "VertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer()
    : m_RendererID(0)
{
}

void VertexBuffer::DeleteBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

VertexBuffer::~VertexBuffer()
{
    DeleteBuffer();
}

void VertexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void VertexBuffer::Init(const void* data, unsigned int size, const bool& sub)
{
    GLCall(glGenBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}
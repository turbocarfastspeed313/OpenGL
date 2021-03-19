#include "index_buffer.h"
#include "renderer.h"

IndexBuffer::IndexBuffer(const void* data, unsigned int count)
	:m_count(count)
{
	GLCall(glGenBuffers(1, &m_RendererID)); //create a buffer
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID)); //"select" a buffer
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * count, data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::bind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::unbind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
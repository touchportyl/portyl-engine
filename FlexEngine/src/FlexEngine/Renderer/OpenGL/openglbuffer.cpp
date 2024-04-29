#pragma once

#include "Renderer/OpenGL/openglbuffer.h"

#include <glad/glad.h>

namespace FlexEngine
{

  #pragma region VertexBuffer

  OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, unsigned int size)
  {
    glGenBuffers(1, &m_renderer_id);
    glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
  }

  OpenGLVertexBuffer::~OpenGLVertexBuffer()
  {
    glDeleteBuffers(1, &m_renderer_id);
  }

  void OpenGLVertexBuffer::Bind() const
  {
    glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
  }

  void OpenGLVertexBuffer::Unbind() const
  {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  #pragma endregion

  #pragma region IndexBuffer

  OpenGLIndexBuffer::OpenGLIndexBuffer(unsigned int* indices, unsigned int count)
    : m_count(count)
  {
    glGenBuffers(1, &m_renderer_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW);
  }

  OpenGLIndexBuffer::~OpenGLIndexBuffer()
  {
    glDeleteBuffers(1, &m_renderer_id);
  }

  void OpenGLIndexBuffer::Bind() const
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id);
  }

  void OpenGLIndexBuffer::Unbind() const
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }

  unsigned int OpenGLIndexBuffer::GetCount() const
  {
    return m_count;
  }

  #pragma endregion

}
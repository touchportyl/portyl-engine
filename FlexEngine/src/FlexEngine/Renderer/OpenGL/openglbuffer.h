#pragma once

#include "flx_api.h"

#include "Renderer/buffer.h"

namespace FlexEngine
{

  #pragma region OpenGLVertexArray

  class __FLX_API OpenGLVertexArray : public VertexArray
  {
    unsigned int m_renderer_id = 0;

  public:
    OpenGLVertexArray();
    virtual ~OpenGLVertexArray();

    virtual void Bind() const;
    virtual void Unbind() const;
  };

  #pragma endregion

  #pragma region OpenGLVertexBuffer

  class __FLX_API OpenGLVertexBuffer : public VertexBuffer
  {
    unsigned int m_renderer_id = 0;

  public:
    OpenGLVertexBuffer(Vertex* vertices, std::size_t size);
    virtual ~OpenGLVertexBuffer();

    virtual void Bind() const;
    virtual void Unbind() const;
  };

  #pragma endregion

  #pragma region OpenGLIndexBuffer

  class __FLX_API OpenGLIndexBuffer : public IndexBuffer
  {
    unsigned int m_renderer_id = 0;
    GLsizei m_count = 0;

  public:
    OpenGLIndexBuffer(unsigned int* indices, GLsizei count);
    virtual ~OpenGLIndexBuffer();

    virtual void Bind() const;
    virtual void Unbind() const;

    virtual GLsizei GetCount() const;
  };

  #pragma endregion

}
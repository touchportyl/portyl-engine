#pragma once

#include "flx_api.h"

#include "Renderer/buffer.h"

namespace FlexEngine
{

  class __FLX_API OpenGLVertexBuffer : public VertexBuffer
  {
    unsigned int m_renderer_id = 0;

  public:
    OpenGLVertexBuffer(float* vertices, unsigned int size);
    virtual ~OpenGLVertexBuffer();

    virtual void Bind() const;
    virtual void Unbind() const;
  };

  class __FLX_API OpenGLIndexBuffer : public IndexBuffer
  {
    unsigned int m_renderer_id = 0;
    unsigned int m_count = 0;

  public:
    OpenGLIndexBuffer(unsigned int* indices, unsigned int count);
    virtual ~OpenGLIndexBuffer();

    virtual void Bind() const;
    virtual void Unbind() const;

    virtual unsigned int GetCount() const;
  };

}
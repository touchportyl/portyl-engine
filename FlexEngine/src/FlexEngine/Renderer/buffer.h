#pragma once

#include "flx_api.h"

#include <memory> // std::shared_ptr

namespace FlexEngine
{

  class __FLX_API VertexBuffer
  {
  public:
    virtual ~VertexBuffer() {}

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    // Best practice to store the pointer in a unique_ptr or shared_ptr
    // Usage: std::unique_ptr<VertexBuffer> vertex_buffer;
    //        vertex_buffer.reset(VertexBuffer::Create(vertices, sizeof(vertices));
    static VertexBuffer* Create(float* vertices, unsigned int size);
  };

  class __FLX_API IndexBuffer
  {
  public:
    virtual ~IndexBuffer() {}

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual unsigned int GetCount() const = 0;

    // Best practice to store the pointer in a unique_ptr or shared_ptr
    // Usage: std::unique_ptr<IndexBuffer> index_buffer;
    //        index_buffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(unsigned int)));
    static IndexBuffer* Create(unsigned int* indices, unsigned int count);
  };

}
#include "Renderer/buffer.h"

#include "Renderer/OpenGL/openglbuffer.h"

namespace FlexEngine
{

  // In the future, support for other graphics APIs can be added here.

  VertexBuffer* VertexBuffer::Create(float* vertices, unsigned int size)
  {
    return new OpenGLVertexBuffer(vertices, size);
  }

  IndexBuffer* IndexBuffer::Create(unsigned int* indices, unsigned int count)
  {
    return new OpenGLIndexBuffer(indices, count);
  }

}

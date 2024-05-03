#include "Renderer/buffer.h"

#include "Renderer/OpenGL/openglbuffer.h"

namespace FlexEngine
{

  // In the future, support for other graphics APIs can be added here.

  VertexArray* VertexArray::Create()
  {
    return new OpenGLVertexArray();
  }

  VertexBuffer* VertexBuffer::Create(Vertex* vertices, std::size_t size)
  {
    return new OpenGLVertexBuffer(vertices, size);
  }

  IndexBuffer* IndexBuffer::Create(unsigned int* indices, GLsizei count)
  {
    return new OpenGLIndexBuffer(indices, count);
  }

}

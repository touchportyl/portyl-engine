#include "openglrenderer.h"

namespace FlexEngine
{

  void Renderer::Clear()
  {
    glClearColor(0.35f, 0.35f, 0.35f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
  }

  void Renderer::Draw(GLsizei size)
  {
    glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, nullptr);
  }

}
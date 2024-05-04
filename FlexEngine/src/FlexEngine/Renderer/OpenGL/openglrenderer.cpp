#include "openglrenderer.h"

namespace FlexEngine
{

  void OpenGLRenderer::EnableDepthTest()
  {
    glEnable(GL_DEPTH_TEST);
  }

  void OpenGLRenderer::DisableDepthTest()
  {
    glDisable(GL_DEPTH_TEST);
  }

  void OpenGLRenderer::ClearFrameBuffer()
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  void OpenGLRenderer::ClearColor(const Vector4& color)
  {
    glClearColor(color.x, color.y, color.z, color.w);
  }

  void OpenGLRenderer::Draw(GLsizei size)
  {
    glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, nullptr);
  }

}
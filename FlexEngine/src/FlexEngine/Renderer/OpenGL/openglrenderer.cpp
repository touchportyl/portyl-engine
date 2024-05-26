#include "openglrenderer.h"

namespace FlexEngine
{

  // static member initialization
  uint32_t OpenGLRenderer::m_draw_calls = 0;
  uint32_t OpenGLRenderer::m_draw_calls_last_frame = 0;

  uint32_t OpenGLRenderer::GetDrawCalls()
  {
    return m_draw_calls;
  }

  uint32_t OpenGLRenderer::GetDrawCallsLastFrame()
  {
    return m_draw_calls_last_frame;
  }

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
    m_draw_calls_last_frame = m_draw_calls;
    m_draw_calls = 0;
  }

  void OpenGLRenderer::Draw(GLsizei size)
  {
    glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, nullptr);
    m_draw_calls++;
  }

}
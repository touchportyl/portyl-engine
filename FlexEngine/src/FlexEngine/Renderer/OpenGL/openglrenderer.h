#pragma once

#include "flx_api.h"

#include "FlexMath/vector4.h"

#include <glad/glad.h>

namespace FlexEngine
{

  class __FLX_API OpenGLRenderer
  {
  public:

    static void EnableDepthTest();
    static void DisableDepthTest();

    static void ClearFrameBuffer();
    static void ClearColor(const Vector4& color);

    static void Draw(GLsizei size);
  };

}
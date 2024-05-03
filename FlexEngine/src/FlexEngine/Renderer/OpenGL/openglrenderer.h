#pragma once

#include "flx_api.h"

#include <glad/glad.h>

namespace FlexEngine
{

  class __FLX_API Renderer
  {
  public:
    static void Clear();
    static void Draw(GLsizei size);
  };

}
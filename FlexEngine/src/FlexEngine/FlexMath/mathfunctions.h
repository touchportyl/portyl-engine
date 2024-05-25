#pragma once

#include "flx_api.h"

namespace FlexEngine
{
  namespace FlexMath
  {

    // Clamps a value between a minimum and maximum.
    template <typename T>
    __FLX_API T Clamp(T value, T min, T max);

  }
}

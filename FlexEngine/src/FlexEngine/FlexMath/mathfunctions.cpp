#include "mathfunctions.h"

namespace FlexEngine
{
  namespace FlexMath
  {

    template <typename T>
    T Clamp(T value, T min, T max)
    {
      return value < min ? min : (value > max ? max : value);
    }

    // Explicit instantiation.

    template __FLX_API int Clamp<int>(int value, int min, int max);
    template __FLX_API float Clamp<float>(float value, float min, float max);
    template __FLX_API double Clamp<double>(double value, double min, double max);

  }
}

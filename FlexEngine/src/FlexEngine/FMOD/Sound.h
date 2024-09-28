#pragma once

#include "flx_api.h"
#include "FMOD/core/fmod.hpp"

namespace FlexEngine
{
  namespace Asset
  {
    // Interface for engine level to pass into wrapped FMOD
    class __FLX_API Sound
    {
    public:
      FMOD::Sound* sound;

      // Only allow construction of sound if we have a key.
      Sound(std::string const& key);
      ~Sound() {};
    };
  }
}
#pragma once

#include "flx_api.h"

#include "windows.h" // BYTE
#undef near
#undef far
#undef min
#undef max

#include <string>
#include <vector>

namespace FlexEngine
{
  namespace Base64
  {

    __FLX_API std::string Encode(const std::vector<BYTE>& data);
    __FLX_API std::vector<BYTE> Decode(const std::string& data);

  }
}

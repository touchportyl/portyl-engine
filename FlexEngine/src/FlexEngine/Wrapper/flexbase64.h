#pragma once

#include "flx_api.h"
#include "flx_windows.h" // BYTE

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

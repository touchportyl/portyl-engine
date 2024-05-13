#pragma once

#include <string>

namespace FlexEngine
{

  // The key is specifically the relative path to the asset from the
  // default directory. This is to ensure that the asset manager can
  // easily find the asset.
  // Example: R"(/images/flexengine/flexengine_logo_black.png)"
  using AssetKey = std::string;

}

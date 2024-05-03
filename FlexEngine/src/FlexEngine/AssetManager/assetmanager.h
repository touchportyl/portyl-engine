#pragma once

#include "flx_api.h"

#include "Wrapper/path.h"

#include "Renderer/OpenGL/opengltextures.h"
#include "Renderer/OpenGL/openglshaders.h"

#include <string>
#include <unordered_map>
#include <variant>

namespace FlexEngine
{

  // The key is specifically the relative path to the asset from the
  // default directory. This is to ensure that the asset manager can
  // easily find the asset.
  // Example: "\images\flexengine\flexengine_logo_black.png"
  using AssetKey = std::string;
  using AssetVariant = std::variant<Asset::Texture, Asset::Shader>;

  // Asset Manager
  // An asset manager must exist. The components should never hold things
  // like mesh data, it should simply point to an already existing asset.
  // 
  // The asset manager will look through the entire directory of assets,
  // parse everything and assign it to a umap.
  // (Basically the File Manager and File List already does this)
  // 
  // A mesh component will simply hold the key (in this case the file path)
  // and perform a lookup everytime it needs it.
  // 
  // The key is specifically the relative path to the asset from the
  // default directory. This is to ensure that the asset manager can
  // easily find the asset.
  class __FLX_API AssetManager
  {
    static Path default_directory;

  public:
    static std::unordered_map<AssetKey, AssetVariant> assets;

    // Load all assets in the directory
    static void Load();

    // Get an asset variant by its key
    static AssetVariant* Get(const AssetKey& key);

    // Helper macro to get an asset by its key
    // Example usage: FLX_ASSET_GET(Asset::Texture, "path/to/texture.png")
    #define FLX_ASSET_GET(TYPE, KEY) std::get<TYPE>(*AssetManager::Get(KEY))

#ifdef _DEBUG
    static void Dump();
#endif
  };

}
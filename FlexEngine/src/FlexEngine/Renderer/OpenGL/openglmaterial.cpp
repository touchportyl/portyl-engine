#include "pch.h"

#include "openglmaterial.h"

#include "AssetManager/assetmanager.h"

namespace FlexEngine
{
  namespace Asset
  {

    Material::Material(TextureVariant _diffuse, TextureVariant _specular, float _shininess)
      : diffuse(_diffuse), specular(_specular), shininess(_shininess)
    {
    }

    #pragma region Getter Functions

    const Texture* Material::GetDiffuse() const
    {
      if (std::holds_alternative<AssetKey>(diffuse))
      {
        return &FLX_ASSET_GET(Asset::Texture, std::get<AssetKey>(diffuse));
      }
      else
      {
        return &std::get<Texture>(diffuse);
      }
    }

    std::pair<const Texture*, float> Material::GetSpecular() const
    {
      if (std::holds_alternative<AssetKey>(specular))
      {
        return { &FLX_ASSET_GET(Asset::Texture, std::get<AssetKey>(specular)), shininess };
      }
      else
      {
        return { &std::get<Texture>(specular), shininess };
      }
    }

    //std::vector<const Texture*> Material::GetDiffuse() const
    //{
    //  std::vector<const Texture*> textures;
    //
    //  for (const auto& variant : diffuse)
    //  {
    //    if (std::holds_alternative<AssetKey>(variant))
    //    {
    //      textures.push_back(&FLX_ASSET_GET(Asset::Texture, std::get<AssetKey>(variant)));
    //    }
    //    else
    //    {
    //      textures.push_back(&std::get<Texture>(variant));
    //    }
    //  }
    //
    //  return textures;
    //}

    //std::vector<const Texture*> Material::GetSpecular() const
    //{
    //  std::vector<const Texture*> textures;
    //
    //  for (const auto& variant : specular)
    //  {
    //    if (std::holds_alternative<AssetKey>(variant))
    //    {
    //      textures.push_back(&FLX_ASSET_GET(Asset::Texture, std::get<AssetKey>(variant)));
    //    }
    //    else
    //    {
    //      textures.push_back(&std::get<Texture>(variant));
    //    }
    //  }
    //
    //  return textures;
    //}

    #pragma endregion

    #pragma region Operator Overloads

    bool Material::operator==(const Material& other) const
    {
      return diffuse == other.diffuse && specular == other.specular;
    }

    bool Material::operator!=(const Material& other) const
    {
      return !(*this == other);
    }

    #pragma endregion

  }
}

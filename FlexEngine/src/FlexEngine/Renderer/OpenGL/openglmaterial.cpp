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
      // guard
      if (diffuse.empty())
      {
        return nullptr;
      }

      return &FLX_ASSET_GET(Asset::Texture, diffuse);
    }

    std::pair<const Texture*, float> Material::GetSpecular() const
    {
      // guard
      if (specular.empty())
      {
        return { nullptr, 0 };
      }

      return { &FLX_ASSET_GET(Asset::Texture, specular), shininess };
    }

    #pragma endregion

    #pragma region Operator Overloads

    bool Material::operator==(const Material& other) const
    {
      return diffuse == other.diffuse && specular == other.specular && shininess == other.shininess;
    }

    bool Material::operator!=(const Material& other) const
    {
      return !(*this == other);
    }

    #pragma endregion

  }
}

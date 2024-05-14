#include "pch.h"

#include "openglmodel.h"

namespace FlexEngine
{
  namespace Asset
  {

    // static member initialization
    Model Model::Null = Model();

    Model::Model(const std::vector<Mesh>& _meshes, const std::vector<Material>& _materials)
      : meshes(_meshes), materials(_materials)
    {
    }

    #pragma region Operator Overloads

    bool Model::operator==(const Model& other) const
    {
      return meshes == other.meshes && materials == other.materials;
    }

    bool Model::operator!=(const Model& other) const
    {
      return !(*this == other);
    }

    Model::operator bool() const
    {
      return !(*this == Null);
    }

    #pragma endregion


  }
}

#include "openglmesh.h"

namespace FlexEngine
{
  namespace Asset
  {

    Mesh::Mesh(
      const std::vector<Vertex>& vertices,
      const std::vector<unsigned int>& indices,
      const Matrix4x4& transform,
      std::size_t material_index,
      const std::string& name
    )
      : vertices(vertices), indices(indices)
      , transform(transform), material_index(material_index)
      , name(name)
    {
    }

    #pragma region Operator Overloads

    bool Mesh::operator==(const Mesh& other) const
    {
      return
        vertices == other.vertices &&
        indices == other.indices &&
        transform == other.transform &&
        material_index == other.material_index &&
        name == other.name
      ;
    }

    bool Mesh::operator!=(const Mesh& other) const
    {
      return !(*this == other);
    }

    #pragma endregion

  }
}
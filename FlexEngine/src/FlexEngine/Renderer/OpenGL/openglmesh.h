#pragma once

#include "flx_api.h"

#include "Renderer/buffer.h"
#include "Renderer/OpenGL/openglvertex.h"
#include "Renderer/OpenGL/openglshader.h"
#include "FlexMath/matrix4x4.h"

#include <vector>

namespace FlexEngine
{
  namespace Asset
  {

    class __FLX_API Mesh
    {
    public:
      std::vector<Vertex> vertices;
      std::vector<unsigned int> indices;
      Matrix4x4 transform;
      std::size_t material_index = 0;
      std::string name = "Unnamed Mesh";

      Mesh() = default;
      Mesh(
        const std::vector<Vertex>& vertices,
        const std::vector<unsigned int>& indices,
        const Matrix4x4& transform,
        std::size_t material_index,
        const std::string& name = "Unnamed Mesh"
      );

      #pragma region Operator Overloads

      bool operator==(const Mesh& other) const;
      bool operator!=(const Mesh& other) const;

      #pragma endregion

    };

  }
}

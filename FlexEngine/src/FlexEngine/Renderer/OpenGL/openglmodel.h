#pragma once

#include "flx_api.h"

#include "Renderer/OpenGL/openglmaterial.h"
#include "Renderer/OpenGL/openglmesh.h"

#include <vector>

namespace FlexEngine
{
  namespace Asset
  {

    class __FLX_API Model
    {
    public:
      static Model Null;
      //static Model Default; // unit cube

      std::vector<Mesh> meshes;
      std::vector<Material> materials;

      Model() = default;
      Model(const std::vector<Mesh>& meshes, const std::vector<Material>& materials);

      #pragma region Operator Overloads
      
      bool operator==(const Model& other) const;
      bool operator!=(const Model& other) const;

      operator bool() const;

      #pragma endregion
    };

  }
}

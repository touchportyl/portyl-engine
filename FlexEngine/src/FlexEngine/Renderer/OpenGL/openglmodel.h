#pragma once

#include "flx_api.h"

#include "openglmesh.h"

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

      Model() = default;
      Model(const std::vector<Mesh>& meshes);

      #pragma region Operator Overloads
      
      bool operator==(const Model& other) const;
      bool operator!=(const Model& other) const;

      operator bool() const;

      #pragma endregion
    };

  }
}

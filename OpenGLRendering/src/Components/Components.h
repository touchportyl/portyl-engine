#pragma once

#include <FlexEngine.h>
using namespace FlexEngine;

// TODO:
// Replace this with my own math library
//#include <glm/glm.hpp>

namespace OpenGLRendering
{

  class Transform
  { FLX_REFL_SERIALIZABLE
  public:
    Matrix4x4 transform;
  };

  class Shader
  { FLX_REFL_SERIALIZABLE
  public:
    AssetKey shader;
  };

  class Texture
  { FLX_REFL_SERIALIZABLE
  public:
    AssetKey texture;
  };

  class Material
  { FLX_REFL_SERIALIZABLE
  public:
    std::vector<AssetKey> diffuse;
    std::vector<AssetKey> specular;
  };

  class Mesh
  { FLX_REFL_SERIALIZABLE
  public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
  };

  class Camera
  { FLX_REFL_SERIALIZABLE
  public:
    Vector3 target_position;
    Vector3 direction;
    Vector3 right;
    Vector3 up;
  };

}
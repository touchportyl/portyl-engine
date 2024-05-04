#pragma once

#include <FlexEngine.h>
using namespace FlexEngine;

namespace OpenGLRendering
{

  class LocalPosition
  { FLX_REFL_SERIALIZABLE
  public:
    Vector3 position;
  };

  class GlobalPosition
  { FLX_REFL_SERIALIZABLE
  public:
    Vector3 position;
  };

  class Rotation
  { FLX_REFL_SERIALIZABLE
  public:
    Vector3 rotation;
  };

  class Scale
  { FLX_REFL_SERIALIZABLE
  public:
    Vector3 scale;
  };

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
    Vector3 front;
    Vector3 right;
    Vector3 up;
    const Vector3 world_up = Vector3::Up;
    Matrix4x4 view;
    Matrix4x4 projection;
    bool perspective = true;
    float fov = 45.0f;
    float near = 0.1f;
    float far = 100.0f;
  };

}
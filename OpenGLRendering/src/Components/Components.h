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

  // For the mesh
  class Material
  { FLX_REFL_SERIALIZABLE
  public:
    AssetKey diffuse;
    std::pair<AssetKey, float> specular;
  };

  // Programmable meshes
  class Mesh
  { FLX_REFL_SERIALIZABLE
  public:
    Asset::Mesh mesh;
  };

  // For imported models
  class Model
  { FLX_REFL_SERIALIZABLE
  public:
    AssetKey model;
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

  class PointLight
  { FLX_REFL_SERIALIZABLE
  public:
    Vector3 ambient;
    Vector3 diffuse;
    Vector3 specular;
  };


  class DirectionalLight
  { FLX_REFL_SERIALIZABLE
  public:
    Vector3 ambient;
    Vector3 diffuse;
    Vector3 specular;
  };

}
#pragma once

#include <FlexEngine.h>
using namespace FlexEngine;

namespace OpenGLRendering
{

  using EntityName = FlexEngine::FlexECS::Scene::StringIndex;

  class IsActive
  { FLX_REFL_SERIALIZABLE
  public:
    bool is_active;
  };

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
    bool is_dirty = true;
    Matrix4x4 transform;
  };

  class Shader
  { FLX_REFL_SERIALIZABLE
  public:
    FlexECS::Scene::StringIndex shader;
  };

  //class Texture
  //{ FLX_REFL_SERIALIZABLE
  //public:
  //  FlexECS::Scene::StringIndex texture;
  //};

  // For the mesh
  //class Material
  //{ FLX_REFL_SERIALIZABLE
  //public:
  //  FlexECS::Scene::StringIndex diffuse;
  //  std::pair<FlexECS::Scene::StringIndex, float> specular;
  //};

  // Programmable meshes
  //class Mesh
  //{ FLX_REFL_SERIALIZABLE
  //public:
  //  Asset::Mesh mesh;
  //};

  // For 2D sprites
  class Sprite
  { FLX_REFL_SERIALIZABLE
  public:
    FlexECS::Scene::StringIndex texture;
    Vector3 color = Vector3(1.0f, 0.0f, 1.0f);
    Vector3 color_to_add = Vector3(0.0f, 0.0f, 0.0f);
    Vector3 color_to_multiply = Vector3(1.0f, 1.0f, 1.0f);
    int alignment = Renderer2DProps::Alignment_Center;
  };

  // For displaying text
  class Text
  { FLX_REFL_SERIALIZABLE
  public:
    FlexECS::Scene::StringIndex font;
    float font_size;
    std::string text;
  };

  // For imported models
  class Model
  { FLX_REFL_SERIALIZABLE
  public:
    FlexECS::Scene::StringIndex model;
  };

  class Camera
  { FLX_REFL_SERIALIZABLE
  public:
    bool is_dirty = true;
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

  class DirectionalLight
  {
    FLX_REFL_SERIALIZABLE
  public:
    Vector3 direction;
    Vector3 ambient;
    Vector3 diffuse;
    Vector3 specular;
  };

  class PointLight
  { FLX_REFL_SERIALIZABLE
  public:
    Vector3 ambient;
    Vector3 diffuse;
    Vector3 specular;
  };

}
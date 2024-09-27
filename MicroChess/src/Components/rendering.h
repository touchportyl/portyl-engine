#pragma once

#include <FlexEngine.h>
using namespace FlexEngine;

namespace MicroChess
{

  using EntityName = FlexEngine::FlexECS::Scene::StringIndex;

  class IsActive
  { FLX_REFL_SERIALIZABLE
  public:
    bool is_active;
  };

  class Parent
  { FLX_REFL_SERIALIZABLE
  public:
    FlexECS::Entity parent;
  };

  class Position
  { FLX_REFL_SERIALIZABLE
  public:
    Vector2 position = Vector2::Zero;
  };

  class Scale
  { FLX_REFL_SERIALIZABLE
  public:
    Vector2 scale = Vector2::One;
  };

  class Rotation 
  {
      FLX_REFL_SERIALIZABLE
  public:
      Vector3 rotation = Vector3::Zero;
  };

  class Transform
  {
      FLX_REFL_SERIALIZABLE
  public:
      bool is_dirty = true;
      Matrix4x4 transform = Matrix4x4::Identity;
  };

  class ZIndex
  { FLX_REFL_SERIALIZABLE
  public:
    int z;
  };

  class Shader
  { FLX_REFL_SERIALIZABLE
  public:
    FlexECS::Scene::StringIndex shader;
  };

  class Sprite
  { FLX_REFL_SERIALIZABLE
  public:
    FlexECS::Scene::StringIndex texture;
    Vector3 color = Vector3(1.0f, 0.0f, 1.0f);
    Vector3 color_to_add = Vector3::Zero;
    Vector3 color_to_multiply = Vector3::One;
    int alignment = Renderer2DProps::Alignment_Center;
  };

  class Camera
  {
      FLX_REFL_SERIALIZABLE
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

}

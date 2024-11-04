/*!************************************************************************
// WLVERSE [https://wlverse.web.app]
// rendering.cpp
//
// This file implements the OpenGLSpriteRenderer class, which is responsible
// for handling 2D sprite rendering within the game engine. It provides
// functions for rendering sprites, applying post-processing effects,
// and managing the necessary OpenGL resources such as shaders and
// framebuffers.
//
// Key functionalities include:
// - Rendering 2D sprites with texture binding and transformations.
// - Supporting post-processing effects such as Gaussian Blur and Bloom.
// - Providing wrapper functions for commonly used OpenGL operations,
//   ensuring streamlined usage across the codebase.
//
// The renderer is built with a focus on performance and flexibility,
// allowing for easy customization and extension of rendering capabilities.
//
// AUTHORS
// [100%] Soh Wei Jie (weijie.soh@digipen.edu)
//   - Main Author
//   - Developed the core rendering functionalities and post-processing
//     pipeline, ensuring compatibility with the game engine's architecture.
//
// Copyright (c) 2024 DigiPen, All rights reserved.
**************************************************************************/
#include "rendering.h"

namespace ChronoShift
{

  FLX_REFL_REGISTER_START(IsActive)
    FLX_REFL_REGISTER_PROPERTY(is_active)
  FLX_REFL_REGISTER_END;

  FLX_REFL_REGISTER_START(Parent)
    FLX_REFL_REGISTER_PROPERTY(parent)
  FLX_REFL_REGISTER_END;

  FLX_REFL_REGISTER_START(Position)
    FLX_REFL_REGISTER_PROPERTY(position)
  FLX_REFL_REGISTER_END;

  FLX_REFL_REGISTER_START(Scale)
    FLX_REFL_REGISTER_PROPERTY(scale)
  FLX_REFL_REGISTER_END;

  FLX_REFL_REGISTER_START(Rotation)
    FLX_REFL_REGISTER_PROPERTY(rotation)
  FLX_REFL_REGISTER_END;

  FLX_REFL_REGISTER_START(Transform)
      FLX_REFL_REGISTER_PROPERTY(is_dirty)
      FLX_REFL_REGISTER_PROPERTY(transform)
  FLX_REFL_REGISTER_END;

  FLX_REFL_REGISTER_START(ZIndex)
    FLX_REFL_REGISTER_PROPERTY(z)
  FLX_REFL_REGISTER_END;

  FLX_REFL_REGISTER_START(Shader)
    FLX_REFL_REGISTER_PROPERTY(shader)
  FLX_REFL_REGISTER_END;
  
  FLX_REFL_REGISTER_START(Sprite)
    FLX_REFL_REGISTER_PROPERTY(texture)
    FLX_REFL_REGISTER_PROPERTY(color_to_add)
    FLX_REFL_REGISTER_PROPERTY(color_to_multiply)
    FLX_REFL_REGISTER_PROPERTY(alignment)
    FLX_REFL_REGISTER_PROPERTY(vbo_id)
    FLX_REFL_REGISTER_PROPERTY(post_processed)
  FLX_REFL_REGISTER_END;

  FLX_REFL_REGISTER_START(Text)
      FLX_REFL_REGISTER_PROPERTY(fonttype)
      FLX_REFL_REGISTER_PROPERTY(text)
      FLX_REFL_REGISTER_PROPERTY(color)
      FLX_REFL_REGISTER_PROPERTY(alignment)
      //FLX_REFL_REGISTER_PROPERTY(vbo_id)
  FLX_REFL_REGISTER_END;



  FLX_REFL_REGISTER_START(Camera)
      FLX_REFL_REGISTER_PROPERTY(is_dirty)
      FLX_REFL_REGISTER_PROPERTY(camera)          // Already registered as it contains CameraData
      //FLX_REFL_REGISTER_PROPERTY(position)
      //FLX_REFL_REGISTER_PROPERTY(direction)
      //FLX_REFL_REGISTER_PROPERTY(up)
      //FLX_REFL_REGISTER_PROPERTY(right)
      //FLX_REFL_REGISTER_PROPERTY(viewMatrix)
      //FLX_REFL_REGISTER_PROPERTY(projMatrix)
      //FLX_REFL_REGISTER_PROPERTY(fieldOfView)
      //FLX_REFL_REGISTER_PROPERTY(aspectRatio)
      //FLX_REFL_REGISTER_PROPERTY(nearClip)
      //FLX_REFL_REGISTER_PROPERTY(farClip)
      //FLX_REFL_REGISTER_PROPERTY(m_isOrthographic)
FLX_REFL_REGISTER_END



//Component registry for editor
//so the properties panel knows how to display them
  COMPONENT_VIEWER_START(Position)
    COMPONENT_VIEWER_DRAG_VECTOR2(position)
  COMPONENT_VIEWER_END(Position)

  COMPONENT_VIEWER_START(Rotation)
    COMPONENT_VIEWER_DRAG_VECTOR3(rotation)
  COMPONENT_VIEWER_END(Rotation)

  COMPONENT_VIEWER_START(Scale)
    COMPONENT_VIEWER_DRAG_VECTOR2(scale)
  COMPONENT_VIEWER_END(Scale)

  COMPONENT_VIEWER_START(IsActive)
    COMPONENT_VIEWER_CHECKBOX(is_active)
  COMPONENT_VIEWER_END(IsActive)

   COMPONENT_VIEWER_START(Transform)
    COMPONENT_VIEWER_BOOL(is_dirty)
    COMPONENT_VIEWER_MAT44(transform)
  COMPONENT_VIEWER_END(Transform)

  COMPONENT_VIEWER_START(ZIndex)
    COMPONENT_VIEWER_DRAG_INT(z)
  COMPONENT_VIEWER_END(ZIndex)

  COMPONENT_VIEWER_START(Parent)
    COMPONENT_VIEWER_ENTITY_REFERENCE(parent)
  COMPONENT_VIEWER_END(Parent)

  COMPONENT_VIEWER_START(Shader)
    COMPONENT_VIEWER_SHADER_PATH(shader)
  COMPONENT_VIEWER_END(Shader)

  COMPONENT_VIEWER_START(Sprite)
    COMPONENT_VIEWER_TEXTURE_PATH(texture)
    COMPONENT_VIEWER_COLOR3(color_to_add)
    COMPONENT_VIEWER_COLOR3(color_to_multiply)
    COMPONENT_VIEWER_CHECKBOX(post_processed)
  COMPONENT_VIEWER_END(Sprite)

  //TODO @ROCKY to add the font pic and have a dropbox of available fonts
  COMPONENT_VIEWER_START(Text)
      COMPONENT_VIEWER_STRING(fonttype)
      COMPONENT_VIEWER_EDITABLE_STRING(text)
      COMPONENT_VIEWER_COLOR3(color)
      //COMPONENT_VIEWER(alignment)
  COMPONENT_VIEWER_END(Text)

  void RegisterRenderingComponents()
  {
    REGISTER_COMPONENT_VIEWER(Position);
    REGISTER_COMPONENT_VIEWER(Rotation);
    REGISTER_COMPONENT_VIEWER(Scale);
    REGISTER_COMPONENT_VIEWER(IsActive);
    REGISTER_COMPONENT_VIEWER(Transform);
    REGISTER_COMPONENT_VIEWER(ZIndex);
    REGISTER_COMPONENT_VIEWER(Parent);
    REGISTER_COMPONENT_VIEWER(Shader);
    REGISTER_COMPONENT_VIEWER(Sprite);
    REGISTER_COMPONENT_VIEWER(Text);
  }
}
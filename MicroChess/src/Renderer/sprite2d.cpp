#include "Renderer/sprite2d.h"

#include "Components/rendering.h"

#define PostProcessing 0
namespace ChronoShift
{

  void RendererSprite2D()
  {
    WindowProps window_props = Application::GetCurrentWindow()->GetProps();
    Renderer2DProps props;
    props.window_size = { static_cast<float>(window_props.width), static_cast<float>(window_props.height) };

    FunctionQueue render_queue;

    if (PostProcessing)
    {
        OpenGLSpriteRenderer::EnablePostProcessing();
        OpenGLSpriteRenderer::ClearFrameBuffer();
    }

    // Render all entities
    for (auto& entity : FlexECS::Scene::GetActiveScene()->View<IsActive, ZIndex, Position, Scale, Rotation, Shader, Sprite>())
    {
      if (!entity.GetComponent<IsActive>()->is_active) continue;

      Vector2 global_position = Vector2::Zero;
      Vector2 global_scale = Vector2::One;
      Vector3 global_rotation = Vector3::Zero;

      if (entity.HasComponent<Parent>())
      {
        auto& parent = entity.GetComponent<Parent>()->parent;
        if (parent.HasComponent<Position>())
        {
          global_position = parent.GetComponent<Position>()->position;
        }
        if (parent.HasComponent<Scale>())
        {
          global_scale = parent.GetComponent<Scale>()->scale;
        }
        if (parent.HasComponent<Rotation>())
        {
           global_rotation = parent.GetComponent<Rotation>()->rotation;
        }
      }

      auto& z_index = entity.GetComponent<ZIndex>()->z;
      auto& position = entity.GetComponent<Position>()->position;
      auto& scale = entity.GetComponent<Scale>()->scale;
      auto& rotation = entity.GetComponent<Rotation>()->rotation;
      auto& shader = FlexECS::Scene::GetActiveScene()->Internal_StringStorage_Get(entity.GetComponent<Shader>()->shader);
      auto sprite = entity.GetComponent<Sprite>();
      
      props.shader = shader;
      props.position = global_position + position/*.RotateDeg(props.rotation.z)*/;
      props.scale = global_scale * scale;
      props.rotation = global_rotation + rotation;
      props.texture = FlexECS::Scene::GetActiveScene()->Internal_StringStorage_Get(sprite->texture);
      props.color = sprite->color;
      props.color_to_add = sprite->color_to_add;
      props.color_to_multiply = sprite->color_to_multiply;
      props.alignment = static_cast<Renderer2DProps::Alignment>(sprite->alignment);

      render_queue.Insert({ [props]() { OpenGLSpriteRenderer::DrawTexture2D(props); }, "", z_index });
    }

    // push settings
    bool depth_test = OpenGLRenderer::IsDepthTestEnabled();
    if (depth_test) OpenGLRenderer::DisableDepthTest();

    bool blending = OpenGLRenderer::IsBlendingEnabled();
    if (!blending) OpenGLRenderer::EnableBlending();

    // batch-render

    render_queue.Flush();
    if (PostProcessing)
    {
        OpenGLSpriteRenderer::DisablePostProcessing();
        OpenGLSpriteRenderer::DrawPostProcessingLayer();
    }

    // pop settings

    if (depth_test) OpenGLRenderer::EnableDepthTest();
    if (!blending) OpenGLRenderer::DisableBlending();
  }

}
#include "Physics/box2d.h"

#include "Components/rendering.h"
#include "Components/game.h"

namespace MicroChess
{

  // update PieceStatus based on mouse input
  void Box2D()
  {
    //WindowProps window_props = Application::GetCurrentWindow()->GetProps();
    //
    //FunctionQueue render_queue;
    //
    //for (auto& entity : FlexECS::Scene::GetActiveScene()->View<IsActive, ZIndex, Parent, Position, Scale, PieceStatus>())
    //{
    //  if (!entity.GetComponent<IsActive>()->is_active) continue;
    //
    //  auto& parent = entity.GetComponent<Parent>()->parent;
    //  auto global_position = parent.HasComponent<Position>() ? parent.GetComponent<Position>()->position : Vector2::Zero;
    //  auto global_scale = parent.HasComponent<Scale>() ? parent.GetComponent<Scale>()->scale : Vector2::One;
    //
    //  auto& z_index = entity.GetComponent<ZIndex>()->z;
    //  auto& position = entity.GetComponent<Position>()->position;
    //  auto& scale = entity.GetComponent<Scale>()->scale;
    //  auto& shader = FlexECS::Scene::GetActiveScene()->Internal_StringStorage_Get(entity.GetComponent<Shader>()->shader);
    //  auto sprite = entity.GetComponent<Sprite>();
    //
    //  // collision detection
    //
    //}

  }

}
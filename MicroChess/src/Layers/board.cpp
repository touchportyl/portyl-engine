#include "board.h"

#include "States.h"
#include "Layers.h"

#include "Components/game.h"
#include "Components/physics.h"
#include "Components/rendering.h"

#include "Physics/box2d.h"
#include "Renderer/sprite2d.h"

namespace MicroChess
{

  void BoardLayer::SetupBoard()
  {
    auto scene = FlexECS::Scene::CreateScene();
    FlexECS::Scene::SetActiveScene(scene);

    // constants

    const Vector3 color_board_white = { 0.92f, 0.93f, 0.82f };
    const Vector3 color_board_black = { 0.45f, 0.58f, 0.32f };
    Window* window = Application::GetCurrentWindow();

    // create entities

    FlexECS::Entity board = FlexECS::Scene::CreateEntity("Board");
    board.AddComponent<IsActive>({ true });
    board.AddComponent<Position>({ {
      (static_cast<float>(window->GetWidth ()) * 0.5f) - (4 * 0.5f * 100.f) + (100.f * 0.5f),
      (static_cast<float>(window->GetHeight()) * 0.5f) - (4 * 0.5f * 100.f) + (100.f * 0.5f)
    } });
    board.AddComponent<Scale>({ { 1, 1 } });

    for (std::size_t i = 0; i < 4; i++) // rows -
    {
      for (std::size_t j = 0; j < 4; j++) // columns |
      {
        std::string name = "Tile" + std::to_string(i * 4 + j);
        FlexECS::Entity tile = FlexECS::Scene::CreateEntity(name);
        tile.AddComponent<IsActive>({ true });
        tile.AddComponent<ZIndex>({ 10 });
        tile.AddComponent<BoardTile>({});
        tile.AddComponent<Parent>({ board });
        tile.AddComponent<Position>({ { j * 100.0f, i * 100.0f } });
        tile.AddComponent<Scale>({ { 100, 100 } });
        tile.AddComponent<Sprite>({
          //scene->Internal_StringStorage_New(R"(\images\chess_king.png)"),
          scene->Internal_StringStorage_New(R"()"),
          ((i + j) % 2 ? color_board_black : color_board_white),
          Vector3::Zero,
          Vector3::One,
          Renderer2DProps::Alignment_Center
        });
        tile.AddComponent<Shader>({ scene->Internal_StringStorage_New(R"(\shaders\texture)") });
      }
    }

    std::array<PieceTypes, 16> pieces = {
      PIECETYPE_ROOK, PIECETYPE_QUEEN, PIECETYPE_KING, PIECETYPE_ROOK,
      PIECETYPE_PAWN, PIECETYPE_PAWN,  PIECETYPE_PAWN, PIECETYPE_PAWN,
      PIECETYPE_PAWN, PIECETYPE_PAWN,  PIECETYPE_PAWN, PIECETYPE_PAWN,
      PIECETYPE_ROOK, PIECETYPE_QUEEN, PIECETYPE_KING, PIECETYPE_ROOK,
    };
    std::array<std::string, 16> piece_asset = {
      R"(\images\chess_rook.png)", R"(\images\chess_queen.png)", R"(\images\chess_king.png)", R"(\images\chess_rook.png)",
      R"(\images\chess_pawn.png)", R"(\images\chess_pawn.png)",  R"(\images\chess_pawn.png)", R"(\images\chess_pawn.png)",
      R"(\images\chess_pawn.png)", R"(\images\chess_pawn.png)",  R"(\images\chess_pawn.png)", R"(\images\chess_pawn.png)",
      R"(\images\chess_rook.png)", R"(\images\chess_queen.png)", R"(\images\chess_king.png)", R"(\images\chess_rook.png)",
    };

    for (std::size_t i = 0; i < 16; i++)
    {
      std::string name = "Piece" + std::to_string(i);
      FlexECS::Entity piece = FlexECS::Scene::CreateEntity(name);
      piece.AddComponent<IsActive>({ true });
      piece.AddComponent<ZIndex>({ 20 });
      piece.AddComponent<IsWhite>({ (i >= 8) });
      piece.AddComponent<PieceType>({ pieces[i] });
      piece.AddComponent<PieceStatus>({});
      piece.AddComponent<Parent>({ board });
      //Vector2 pos = { static_cast<int>(i / 4), i % 4 };
      //piece.AddComponent<PiecePosition>({ pos });
      piece.AddComponent<Position>({ { (i % 4) * 100.0f, (i / 4) * 100.0f } });
      piece.AddComponent<Scale>({ { 50, 50 } });
      piece.AddComponent<Sprite>({
        scene->Internal_StringStorage_New(piece_asset[i]),
        Vector3::One,
        Vector3::Zero,
        (i >= 8) ? Vector3::One : Vector3::Zero,
        Renderer2DProps::Alignment_Center
      });
      piece.AddComponent<Shader>({ scene->Internal_StringStorage_New(R"(\shaders\texture)") });
      piece.AddComponent<BoundingBox2D>({ Vector2::One });
      piece.AddComponent<OnHover>({});
    }

  }

  void BoardLayer::OnAttach()
  {
    FLX_FLOW_BEGINSCOPE();

    // ECS Setup
    SetupBoard();
  }

  void BoardLayer::OnDetach()
  {
    FLX_FLOW_ENDSCOPE();
  }

  void BoardLayer::Update()
  {
    Box2D();

    // make the piece bigger when hovered
    for (auto& entity : FlexECS::Scene::GetActiveScene()->View<IsActive, OnHover, Scale>())
    {
      if (!entity.GetComponent<IsActive>()->is_active) continue;
    
      auto on_hover = entity.GetComponent<OnHover>();
      auto& scale = entity.GetComponent<Scale>()->scale;
    
      if (on_hover->on_enter) scale *= Vector2(1.5f, 1.5f);
      if (on_hover->on_exit) scale /= Vector2(1.5f, 1.5f);
    }

    RendererSprite2D();
  }

}

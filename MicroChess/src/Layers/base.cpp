/* Start Header
*****************************************************************/
/*!
WLVERSE [https://wlverse.web.app]
\file      base.cpp
\author    [100%] Rocky Sutarius
\par       rocky.sutarius\@digipen.edu
\date      03 October 2024
\brief     This file 

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/* End Header
*******************************************************************/
#include "base.h"

#include "States.h"
#include "Layers.h"

#include "Components/rendering.h"

namespace ChronoShift
{

  void BaseLayer::OnAttach()
  {
    FLX_FLOW_BEGINSCOPE();

    // load assets only after the window has been created
    AssetManager::Load();
    FreeQueue::Push(std::bind(&AssetManager::Unload), "MicroChess AssetManager");

    FlexEngine::Window* window = Application::GetCurrentWindow();
    window->SetTargetFPS();
    window->SetVSync(false);
    window->SetIcon(FLX_ASSET_GET(Asset::Texture, R"(\images\flexengine\flexengine_icon_white.png)"));

    //window->PushLayer(std::make_shared<MenuLayer>());
    //window->PushLayer(std::make_shared<BoardLayer>());
    //window->PushLayer(std::make_shared<ChronoShift::BattleLayer>());
    window->PushLayer(std::make_shared<ChronoShift::OverworldLayer>());
    window->PushLayer(std::make_shared<ChronoShift::EditorLayer>());

    // Renderer Setup

    OpenGLRenderer::EnableBlending();
    Vector2 windowsize{ static_cast<float>(window->GetWidth()), static_cast<float>(window->GetHeight()) };
    OpenGLSpriteRenderer::Init(windowsize);
  }

  void BaseLayer::OnDetach()
  {
    FLX_FLOW_ENDSCOPE();

    OpenGLRenderer::DisableBlending();

    FreeQueue::RemoveAndExecute("OpenGLRendering AssetManager");
  }

  void BaseLayer::Update()
  {
    #pragma region M1 Controls
    if (Input::GetKeyDown(GLFW_KEY_1))
    {
      FMODWrapper::Core::PlaySound("mario", FLX_ASSET_GET(Asset::Sound, AssetKey("/audio/test.mp3")));
    }
    if (Input::GetKeyDown(GLFW_KEY_2))
    {
      FMODWrapper::Core::StopSound("mario");
    }
    #pragma endregion
    OpenGLRenderer::ClearFrameBuffer();

    FunctionQueue function_queue;

    // setup dockspace
    ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_NoDockingInCentralNode;
    #pragma warning(suppress: 4189) // local variable is initialized but not referenced
    ImGuiID dockspace_main_id = ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), dockspace_flags);
    

    #pragma region Title Bar

    #if 1
    {
      ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 8.0f));
      ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(16.0f, 8.0f));

      if (ImGui::BeginMainMenuBar())
      {
        // center the logo
        float logo_scl = 25.f;
        float prev_cursor_y = ImGui::GetCursorPosY();
        float cursor_y = prev_cursor_y + (ImGui::GetFrameHeight() - logo_scl) * 0.5f;
        ImGui::SetCursorPosY(cursor_y);

        // display the flexengine logo
        Asset::Texture& flexengine_logo = FLX_ASSET_GET(Asset::Texture, AssetKey(R"(\images\flexengine\flexengine_logo_white.png)"));
        ImGui::Image(flexengine_logo.GetTextureImGui(), ImVec2((logo_scl / 7 * 20), logo_scl));

        // reset the cursor position
        ImGui::SetCursorPosY(prev_cursor_y);

        if (ImGui::BeginMenu("File"))
        {
          if (ImGui::MenuItem("New", "Ctrl+N"))
          {
            function_queue.Insert({
              [this]()
              {
                //CreateDefaultScene();
              }
            });
          }

          if (ImGui::MenuItem("Open", "Ctrl+O"))
          {
            function_queue.Insert({
              [this]()
              {
                FileList files = FileList::Browse(
                    "Open Scene",
                    Path::current("saves"), "default.flxscene",
                    L"FlexScene Files (*.flxscene)\0" L"*.flxscene\0",
                    //L"All Files (*.*)\0" L"*.*\0",
                    false,
                    false
                );

                // cancel if no files were selected
                if (files.empty()) return;

                // open the scene
                File& file = File::Open(files[0]);

                // update the current save directory and name
                current_save_directory = file.path.parent_path();
                current_save_name = file.path.stem().string();

                // load the scene
                FlexECS::Scene::SetActiveScene(FlexECS::Scene::Load(file));
                Log::Info("Loaded scene from: " + file.path.string());
              }
            });
          }

          if (ImGui::MenuItem("Save", "Ctrl+S"))
          {
            function_queue.Insert({
              [this]()
              {
                Path current_save_path = current_save_directory.append(current_save_name + ".flxscene");
                FlexECS::Scene::SaveActiveScene(File::Open(current_save_path));
                Log::Info("Saved scene to: " + current_save_path.string());
              }
            });
          }

          if (ImGui::MenuItem("Save As", "Ctrl+Shift+S"))
          {
            function_queue.Insert({
              [this]()
              {
                FileList files = FileList::Browse(
                    "Save Scene",
                    current_save_directory, current_save_name + ".flxscene",
                    L"FlexScene Files (*.flxscene)\0" L"*.flxscene\0"
                    L"All Files (*.*)\0" L"*.*\0",
                    true
                );

                // cancel if no files were selected
                if (files.empty()) return;

                // sanitize the file extension
                std::string file_path = files[0];
                auto it = file_path.find_last_of(".");
                if (it != std::string::npos)
                {
                  file_path = file_path.substr(0, it);
                }
                file_path += ".flxscene";

                // open the file
                File& file = File::Open( Path(file_path) );

                // save the scene
                FlexECS::Scene::SaveActiveScene(file);
                Log::Info("Saved scene to: " + file.path.string());

                // update the current save directory and name
                current_save_directory = file.path.parent_path();
                current_save_name = file.path.stem().string();
              }
            });
          }

          //if (ImGui::MenuItem("Reload", "Ctrl+R")) {}
          //if (ImGui::MenuItem("Close", "Ctrl+W")) {}

          if (ImGui::MenuItem("Exit", "Ctrl+Q"))
          {
            Application::Close();
          }

          ImGui::EndMenu();
        }

//if (ImGui::BeginMenu("Edit"))
//{
//  if (ImGui::MenuItem("Undo", "Ctrl+Z")) {}
//  if (ImGui::MenuItem("Redo", "Ctrl+Y")) {}
//  ImGui::EndMenu();
//}

if (ImGui::BeginMenu("View"))
{
  if (ImGui::MenuItem("Center Window"))
  {
    function_queue.Insert({
      [this]()
      {
        Application::GetCurrentWindow()->CenterWindow();
      }
    });
  }

  ImGui::EndMenu();
}

ImGui::EndMainMenuBar();
      }

      ImGui::PopStyleVar(2);

      // Title bar window drag functionality
      #pragma region Window Drag Functionality

      static bool is_dragging_window = false;

      ImGuiIO& io = ImGui::GetIO();
      ImVec2 mouse_pos = io.MousePos;

      // The main menu bar height is 30 pixels
      ImGuiViewport* viewport = ImGui::GetMainViewport();
      ImVec2 top_left = viewport->Pos;
      ImVec2 bottom_right = ImVec2(top_left.x + viewport->Size.x, top_left.y + 30.0f);

      // Window dragging state
      if (io.MouseDown[0])
      {
        if (
          io.MouseClicked[0] &&
          mouse_pos.x >= top_left.x && mouse_pos.x <= bottom_right.x &&
          mouse_pos.y >= top_left.y && mouse_pos.y <= bottom_right.y
        )
        {
          is_dragging_window = true;
        }
      }
      else
      {
        is_dragging_window = false;
      }

      // Dragging the window
      if (is_dragging_window)
      {
        Window* current_window = Application::GetCurrentWindow();
        if (current_window)
        {
          int pos_x, pos_y;
          current_window->GetWindowPosition(&pos_x, &pos_y);

          // We can't use Input::GetCursorPositionDelta() because it only handles within the glfw window
          // ImGui's IO MousePos is in global screen coordinates

          //Vector2 mouse_delta = Input::GetCursorPositionDelta();
          //if (mouse_delta.x != 0.0f || mouse_delta.y != 0.0f)
          //{
          //  current_window->SetWindowPosition(
          //    pos_x + static_cast<int>(mouse_delta.x),
          //    pos_y + static_cast<int>(mouse_delta.y)
          //  );
          //}

          if (io.MouseDelta.x != 0.0f || io.MouseDelta.y != 0.0f)
          {
            current_window->SetWindowPosition(
              pos_x + static_cast<int>(io.MouseDelta.x),
              pos_y + static_cast<int>(io.MouseDelta.y)
            );
          }
        }
      }

      #pragma endregion

    }
    #endif

    #pragma endregion


    #pragma region Statistics Panel

    #if 1
    {
      ImGui::Begin("Statistics");
      ImGuiTreeNodeFlags tree_node_flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnDoubleClick; // open by default

      if (ImGui::CollapsingHeader("Window", tree_node_flags))
      {
        auto window = Application::GetCurrentWindow();
        int win_pos_x, win_pos_y;
        window->GetWindowPosition(&win_pos_x, &win_pos_y);
        ImGui::Text("Window Position: %d, %d", win_pos_x, win_pos_y);
        ImGui::Text("Window Size: %d x %d", window->GetWidth(), window->GetHeight());
        ImGui::Text("FPS: %d", window->GetFPS());
        ImGui::Text("Delta Time: %.3f ms", window->GetDeltaTime());
      }

      if (ImGui::CollapsingHeader("Scene", tree_node_flags))
      {
        ImGui::Text("Active Scene: %s", current_save_name.c_str());
        ImGui::Text("Entities: %d", FlexECS::Scene::GetActiveScene()->View<EntityName>().size());
        ImGui::Text("Archetypes: %d", ARCHETYPE_INDEX.size());
      }

      if (ImGui::CollapsingHeader("Renderer", tree_node_flags))
      {
        ImGui::Text("Draw Calls: %d", OpenGLRenderer::GetDrawCallsLastFrame());
      }

      ImGui::End();
    }
    #endif

    #pragma endregion


    function_queue.Flush();

  }

}

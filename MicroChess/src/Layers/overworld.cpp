#include "overworld.h"

#include "States.h"
#include "Layers.h"

#include "Components/battlecomponents.h"
#include "Components/physics.h"
#include "Components/rendering.h"
#include "Components/characterinput.h"

#include "BattleSystems/physicssystem.h"

#include "Renderer/sprite2d.h"

#include "Renderer/camera2d.h"

namespace ChronoShift {
    float OverworldLayer::m_ScaleDebugTest = 0.8f;
    Vector3 OverworldLayer::m_RotateDebugTest = Vector3(0.f, 0.f, 0.f);

    void OverworldLayer::SetupWorld()
    {
        auto scene = FlexECS::Scene::GetActiveScene();

        //Camera 
        #if 1
        FlexECS::Entity camera = FlexECS::Scene::CreateEntity("MainCamera");
        camera.AddComponent<IsActive>({ true });
        camera.AddComponent<Position>({ {-150, 300 } });
        camera.AddComponent<Scale>({ { 0.5,0.5 } });
        camera.AddComponent<Rotation>({ });
        camera.AddComponent<Transform>({});
        camera.AddComponent<Camera>({});
        CameraManager::AddCameraEntity(camera.Get(), camera.GetComponent<Camera>()->camera);
        CameraManager::SwitchMainCamera(camera.Get());

        #endif
    }



    void OverworldLayer::OnAttach()
    {
        FLX_FLOW_BEGINSCOPE();

        // ECS Setup
        auto scene = FlexECS::Scene::CreateScene();
        FlexECS::Scene::SetActiveScene(scene);

        // Setup for level editor
        RegisterRenderingComponents();
        RegisterPhysicsComponents();

        SetupWorld();
    }

    void OverworldLayer::OnDetach()
    {
        FLX_FLOW_ENDSCOPE();
    }

  void OverworldLayer::Update()
  {
      Profiler profiler;
      profiler.StartCounter("Misc");
      for (auto& entity : FlexECS::Scene::GetActiveScene()->CachedQuery<CharacterInput>())
      {
          entity.GetComponent<CharacterInput>()->up = Input::GetKey(GLFW_KEY_W);
          entity.GetComponent<CharacterInput>()->down = Input::GetKey(GLFW_KEY_S);
          entity.GetComponent<CharacterInput>()->left = Input::GetKey(GLFW_KEY_A);
          entity.GetComponent<CharacterInput>()->right = Input::GetKey(GLFW_KEY_D);
      }

      for (auto& entity : FlexECS::Scene::GetActiveScene()->CachedQuery<CharacterInput, Rigidbody>())
      {
          auto& velocity = entity.GetComponent<Rigidbody>()->velocity;
          velocity.x = 0.0f;
          velocity.y = 0.0f;

          if (entity.GetComponent<CharacterInput>()->up)
          {
              velocity.y = -300.0f;
          }

          if (entity.GetComponent<CharacterInput>()->down)
          {
              velocity.y = 300.0f;
          }

          if (entity.GetComponent<CharacterInput>()->left)
          {
              velocity.x = -300.0f;
          }

          if (entity.GetComponent<CharacterInput>()->right)
          {
              velocity.x = 300.0f;
          }
        }
      profiler.EndCounter("Misc");

      //For testing 2500 objects
      //Create one, then clone the rest
      if (Input::GetKeyDown(GLFW_KEY_0))
      {
          auto scene = FlexECS::Scene::GetActiveScene();

            FlexECS::Entity thing = FlexECS::Scene::CreateEntity("White Queen");
            thing.AddComponent<IsActive>({ true });
            thing.AddComponent<Position>({ {0,0} });
            thing.AddComponent<Rotation>({ });
            thing.AddComponent<Scale>({ { 15,15 } });
            thing.AddComponent<ZIndex>({ 10 });
            thing.AddComponent<Transform>({ });
            thing.AddComponent<Sprite>({
              scene->Internal_StringStorage_New(R"(\images\chess_queen.png)"),
              Vector3::One,
              Vector3::Zero,
              Vector3::One,
              Renderer2DProps::Alignment_Center
             });
            thing.AddComponent<Shader>({ scene->Internal_StringStorage_New(R"(\shaders\texture)") });

            for (size_t x = 0; x < 50; x++)
            {
                for (size_t y = 0; y < 50; y++)
                {
                    FlexECS::Entity cloned_thing = scene->CloneEntity(thing);
                    auto& position = cloned_thing.GetComponent<Position>()->position;
                    position.x = static_cast<float>(15 * (x + 1));
                    position.y = static_cast<float>(15 * (y + 1));
                }
            }
        }

      if (Input::GetKeyDown(GLFW_KEY_5))
      {
        FMODWrapper::Core::PlaySound("ding", FLX_ASSET_GET(Asset::Sound, AssetKey("/audio/ding-126626.mp3")));
      }
      if (Input::GetKeyDown(GLFW_KEY_6))
      {
        FMODWrapper::Core::PlaySound("boom", FLX_ASSET_GET(Asset::Sound, AssetKey("/audio/big-cine-boom-sound-effect-245851.mp3")));
      }
      if (Input::GetKeyDown(GLFW_KEY_7))
      {
        FMODWrapper::Core::PlaySound("wow", FLX_ASSET_GET(Asset::Sound, AssetKey("/audio/wow-171498.mp3")));
      }
      if (Input::GetKeyDown(GLFW_KEY_8))
      {
        FMODWrapper::Core::PlayLoopingSound("mario", FLX_ASSET_GET(Asset::Sound, AssetKey("/audio/mario.mp3")));
      }
      if (Input::GetKeyDown(GLFW_KEY_9))
      {
        FMODWrapper::Core::StopSound("mario");
      }

      profiler.StartCounter("Physics");
      UpdatePhysicsSystem();
      profiler.EndCounter("Physics");

      // System to handle button collider callbacks
      for (auto& element : FlexECS::Scene::GetActiveScene()->CachedQuery<Button, Sprite>())
      {
        if (!element.GetComponent<IsActive>()->is_active) continue;

        Vector2 mtw = Editor::GetInstance().GetPanel("GameView").mouse_to_world;
        BoundingBox2D bb = *element.GetComponent<BoundingBox2D>();
        if (mtw.x > bb.min.x && mtw.x < bb.max.x && mtw.y > bb.min.y && mtw.y < bb.max.y)
        {
          element.GetComponent<Sprite>()->color_to_add.x = 255;
        }
        else
        {
          element.GetComponent<Sprite>()->color_to_add.x = 0;
        }
      }

      #pragma region Camera Movement -> Should be moved to scripting

      FlexECS::Entity cam_entity = CameraManager::GetMainCamera();
      auto& curr_cam = cam_entity.GetComponent<Position>()->position;
      auto& curr_camt = cam_entity.GetComponent<Transform>()->is_dirty;
      if (Input::GetKey(GLFW_KEY_UP))
      {
          curr_cam += Vector2(0.0f, -5.f)* (30 * FlexEngine::Application::GetCurrentWindow()->GetDeltaTime());
          curr_camt = true;
      }
      else if (Input::GetKey(GLFW_KEY_DOWN))
      {
          curr_cam += Vector2(0.0f, 5.f) * (30 * FlexEngine::Application::GetCurrentWindow()->GetDeltaTime());
          curr_camt = true;
      }

      if (Input::GetKey(GLFW_KEY_RIGHT))
      {
          curr_cam += Vector2(5.f, 0.0f) * (30 * FlexEngine::Application::GetCurrentWindow()->GetDeltaTime());
          curr_camt = true;
      }
      else if (Input::GetKey(GLFW_KEY_LEFT))
      {
          curr_cam += Vector2(-5.f, 0.0f) * (30 * FlexEngine::Application::GetCurrentWindow()->GetDeltaTime());
          curr_camt = true;
      }

      // Regen Cam
      //for (auto& currCam : FlexECS::Scene::GetActiveScene()->CachedQuery<IsActive, Camera>())
      //{
      //    if (!CameraManager::HasCameraEntity(currCam.Get()))
      //    {
      //        CameraManager::AddCameraEntity(currCam.Get(), currCam.GetComponent<Camera>()->camera);
      //        CameraManager::SwitchMainCamera(currCam.Get());
      //    }
      //}
      #pragma endregion
      
      //Render All Entities
      profiler.StartCounter("Graphics");
      RendererSprite2D();
      profiler.EndCounter("Graphics");

      profiler.ShowProfilerWindow();
    }
}

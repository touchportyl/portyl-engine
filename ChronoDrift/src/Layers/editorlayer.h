#pragma once

#include <FlexEngine.h>

using namespace FlexEngine;

namespace ChronoDrift
{

  class EditorLayer : public FlexEngine::Layer
  {
    const Path default_save_directory = Path::current("saves");
    const std::string default_save_name = "default";
    Path current_save_directory = default_save_directory;
    std::string current_save_name = default_save_name;

    bool show_colliders = false;

    CameraManager* m_CamM_Instance = nullptr;
  public:
      EditorLayer() = delete;
    EditorLayer(CameraManager* CamManager) : Layer("Debug Layer"), m_CamM_Instance(CamManager) {}
    ~EditorLayer() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void Update() override;
  };

}

#pragma once

#include <FlexEngine.h>
using namespace FlexEngine;

namespace OpenGLRendering
{

  class MainLayer : public FlexEngine::Layer
  {
    FlexECS::Entity main_camera;
    FlexECS::Entity directional_light;
    std::vector<FlexECS::Entity> point_lights;

    FlexECS::Entity plane;
    FlexECS::Entity object;

    Path current_save_path = Path::current("saves/default.flxscene");

  public:
    MainLayer() : Layer("Main Layer") {}
    ~MainLayer() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void Update() override;

  private:
    void CreateDefaultScene();
  };

}

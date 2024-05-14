#pragma once

#include <FlexEngine.h>
using namespace FlexEngine;

namespace OpenGLRendering
{

  class MainLayer : public FlexEngine::Layer
  {
    FlexECS::Entity main_camera;
    FlexECS::Entity directional_light;
    FlexECS::Entity plane;
    FlexECS::Entity cube;
    FlexECS::Entity object;

  public:
    MainLayer() : Layer("Main Layer") {}
    ~MainLayer() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void Update() override;
  };

}

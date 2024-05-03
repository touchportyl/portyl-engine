#pragma once

#include <FlexEngine.h>
using namespace FlexEngine;

namespace OpenGLRendering
{

  class MainLayer : public FlexEngine::Layer
  {
    FlexECS::Entity camera;
    FlexECS::Entity cube;
    FlexECS::Entity plane;

    //Asset::Shader shader_asset;
    //Asset::Texture texture_asset;

  public:
    MainLayer() : Layer("Main Layer") {}
    ~MainLayer() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void Update() override;
  };

}
